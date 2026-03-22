#include "../include/Data.hpp"
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <fstream>
#include <memory>
#include <string>
#include <sstream>
#include <thread>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <iomanip>

#include <ixwebsocket/IXWebSocket.h>
#include <nlohmann/json.hpp>

namespace
{
    std::vector<uint32_t> Utf8ToCodepoints(const std::string &input)
    {
        std::vector<uint32_t> out;
        out.reserve(input.size());

        for (std::size_t i = 0; i < input.size();)
        {
            const unsigned char c = static_cast<unsigned char>(input[i]);
            if (c < 0x80)
            {
                out.push_back(c);
                ++i;
            }
            else if ((c & 0xE0) == 0xC0 && i + 1 < input.size())
            {
                const uint32_t cp = ((c & 0x1F) << 6) |
                    (static_cast<unsigned char>(input[i + 1]) & 0x3F);
                out.push_back(cp);
                i += 2;
            }
            else if ((c & 0xF0) == 0xE0 && i + 2 < input.size())
            {
                const uint32_t cp = ((c & 0x0F) << 12) |
                    ((static_cast<unsigned char>(input[i + 1]) & 0x3F) << 6) |
                    (static_cast<unsigned char>(input[i + 2]) & 0x3F);
                out.push_back(cp);
                i += 3;
            }
            else if ((c & 0xF8) == 0xF0 && i + 3 < input.size())
            {
                const uint32_t cp = ((c & 0x07) << 18) |
                    ((static_cast<unsigned char>(input[i + 1]) & 0x3F) << 12) |
                    ((static_cast<unsigned char>(input[i + 2]) & 0x3F) << 6) |
                    (static_cast<unsigned char>(input[i + 3]) & 0x3F);
                out.push_back(cp);
                i += 4;
            }
            else
            {
                out.push_back(c);
                ++i;
            }
        }

        return out;
    }

    void AppendUtf8(std::string &out, uint32_t cp)
    {
        if (cp <= 0x7F)
        {
            out.push_back(static_cast<char>(cp));
            return;
        }
        if (cp <= 0x7FF)
        {
            out.push_back(static_cast<char>(0xC0 | ((cp >> 6) & 0x1F)));
            out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
            return;
        }
        if (cp <= 0xFFFF)
        {
            out.push_back(static_cast<char>(0xE0 | ((cp >> 12) & 0x0F)));
            out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
            return;
        }

        out.push_back(static_cast<char>(0xF0 | ((cp >> 18) & 0x07)));
        out.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    }

    std::string DecodeBlitzPayload(const std::string &encoded)
    {
        const std::vector<uint32_t> codes = Utf8ToCodepoints(encoded);
        if (codes.empty())
        {
            return {};
        }

        std::unordered_map<int, std::u32string> dict;
        std::u32string current(1, static_cast<char32_t>(codes[0]));
        std::u32string previous = current;
        std::u32string decoded = current;
        int nextCode = 256;

        for (std::size_t i = 1; i < codes.size(); ++i)
        {
            const int code = static_cast<int>(codes[i]);
            std::u32string entry;

            if (code < 256)
            {
                entry.push_back(static_cast<char32_t>(code));
            }
            else if (dict.find(code) != dict.end())
            {
                entry = dict[code];
            }
            else
            {
                entry = previous;
                entry.push_back(current.front());
            }

            decoded += entry;
            current.clear();
            current.push_back(entry.front());
            dict[nextCode] = previous + current;
            ++nextCode;
            previous = entry;
        }

        std::string utf8;
        utf8.reserve(decoded.size());
        for (char32_t cp : decoded)
        {
            AppendUtf8(utf8, static_cast<uint32_t>(cp));
        }
        return utf8;
    }
}

Data::Data(const ShaderProgram &shader)
    : elapsedTimeSec_(0.0f),
    maxAgeSec_(900.0f),
    websocketStarted_(false)
{
    shader_ = std::make_unique<ShaderProgram>(shader);
    InitData();
    GetBlitzortung();
}

Data::~Data()
{
    if (websocket_ != nullptr)
    {
        websocket_->stop();
        websocket_.reset();
    }

    dataVBO_.destroy();
    dataVAO_.destroy();
}

glm::vec2 Data::LatLonToScreen(float lat, float lon, float width, float height)
{
    const float u = (lon + 180.0f) / 360.0f;
    const float v = (90.0f - lat) / 180.0f;
    return glm::vec2(u * width, v * height);
}

void Data::RefreshVertexData(float width, float height)
{
    vertexData_.clear();
    vertexData_.reserve(strikes_.size() * 5);

    for (const LightningStrike &strike : strikes_)
    {
        const glm::vec2 screenPos = LatLonToScreen(strike.latitude,
                                                    strike.longitude,
                                                    width,
                                                    height);
        const float ageSec = std::max(0.0f, elapsedTimeSec_ - strike.eventTimeSec);
        const float ageNorm = std::min(ageSec / maxAgeSec_, 1.0f);

        vertexData_.push_back(screenPos.x);
        vertexData_.push_back(screenPos.y);
        vertexData_.push_back(ageNorm);
        vertexData_.push_back(strike.intensityNorm);
        vertexData_.push_back(strike.polarity);
    }

    dataVBO_.bind();
    dataVBO_.setData(vertexData_, GL_DYNAMIC_DRAW);
    VBO::unbind();
}


void Data::GetBlitzortung()
{
    if (websocketStarted_)
    {
        return;
    }

    // Official Blitzortung map feed handshake from live_lightning_maps.js.
    static const char *kGlobalFeedSubscription = R"({"a":111})";

    websocket_ = std::make_unique<ix::WebSocket>();
    websocket_->setUrl("wss://ws1.blitzortung.org");

    websocket_->setOnMessageCallback([this](const ix::WebSocketMessagePtr &msg)
    {
        auto tryExtractStrike = [](const nlohmann::json &node, LightningStrike &out) -> bool
        {
            if (!node.is_object())
            {
                return false;
            }

            const bool hasLatLon = node.contains("lat") && node.contains("lon");
            const bool hasLatitudeLongitude = node.contains("latitude") && node.contains("longitude");
            if (!hasLatLon && !hasLatitudeLongitude)
            {
                return false;
            }

            float latitude = hasLatLon ? node["lat"].get<float>() : node["latitude"].get<float>();
            float longitude = hasLatLon ? node["lon"].get<float>() : node["longitude"].get<float>();

            // Blitzortung can send fine offset corrections in some payloads.
            if (node.contains("latc")) latitude += node["latc"].get<float>();
            if (node.contains("lonc")) longitude += node["lonc"].get<float>();
            if (latitude < -90.0f || latitude > 90.0f || longitude < -180.0f || longitude > 180.0f)
            {
                return false;
            }

            out.latitude = latitude;
            out.longitude = longitude;
            out.intensityNorm = std::clamp(node.value("mds", node.value("intensity", 1.0f)), 0.0f, 20.0f) / 20.0f;
            out.polarity = node.value("pol", node.value("polarity", 0)) == 1 ? 1.0f : -1.0f;
            out.eventTimeSec = 0.0f;
            out.age = 0.0f;
            out.timestamp = std::chrono::steady_clock::now();
            return true;
        };

        if (msg->type == ix::WebSocketMessageType::Open)
        {
            websocket_->send(kGlobalFeedSubscription);
            std::cout << "Blitzortung websocket connected" << std::endl;
            return;
        }

        if (msg->type == ix::WebSocketMessageType::Error)
        {
            std::cerr << "Blitzortung websocket error: " << msg->errorInfo.reason << std::endl;
            return;
        }

        if (msg->type == ix::WebSocketMessageType::Close)
        {
            std::cerr << "Blitzortung websocket closed" << std::endl;
            return;
        }

        if (msg->type != ix::WebSocketMessageType::Message)
        {
            return;
        }

        auto j = nlohmann::json::parse(msg->str, nullptr, false);
        if (j.is_discarded())
        {
            const std::string decodedPayload = DecodeBlitzPayload(msg->str);
            if (!decodedPayload.empty())
            {
                j = nlohmann::json::parse(decodedPayload, nullptr, false);
            }

            if (!j.is_discarded())
            {
                static int decodedPayloadSamples = 0;
                if (decodedPayloadSamples < 3)
                {
                    ++decodedPayloadSamples;
                    std::cout << "Decoded Blitz payload sample " << decodedPayloadSamples
                              << ": " << decodedPayload.substr(0, 180) << std::endl;
                }
            }
        }

        if (j.is_discarded())
        {
            static int parseFailureSamples = 0;
            if (parseFailureSamples < 5)
            {
                ++parseFailureSamples;
                std::cout << "Non-JSON websocket payload sample " << parseFailureSamples
                          << " (binary=" << (msg->binary ? "true" : "false")
                          << ", size=" << msg->str.size() << "): "
                          << msg->str.substr(0, 180) << std::endl;
            }
            return;
        }

        std::vector<LightningStrike> parsed;
        parsed.reserve(4);

        auto collectFromNode = [&](const nlohmann::json &node)
        {
            LightningStrike s{};
            if (tryExtractStrike(node, s))
            {
                parsed.push_back(std::move(s));
            }
        };

        if (j.is_array())
        {
            for (const auto &item : j)
            {
                collectFromNode(item);
            }
        }
        else if (j.is_object())
        {
            collectFromNode(j);

            if (parsed.empty() && j.contains("strikes") && j["strikes"].is_array())
            {
                for (const auto &item : j["strikes"])
                {
                    collectFromNode(item);
                }
            }

            if (parsed.empty() && j.contains("data") && j["data"].is_array())
            {
                for (const auto &item : j["data"])
                {
                    collectFromNode(item);
                }
            }
        }

        if (parsed.empty())
        {
            static int ignoredPayloadSamples = 0;
            if (ignoredPayloadSamples < 5)
            {
                ++ignoredPayloadSamples;
                std::cout << "Ignoring websocket payload sample " << ignoredPayloadSamples
                          << ": " << msg->str.substr(0, 180) << std::endl;
            }
            return;
        }

        {
            std::lock_guard<std::mutex> lock(queueMutex_);
            for (auto &s : parsed)
            {
                strikeQueue_.emplace(std::move(s));
            }
        }
    });

    websocket_->start();
    websocketStarted_ = true;
}

void Data::PruneExpiredStrikes()
{
    strikes_.erase(
        std::remove_if(strikes_.begin(), strikes_.end(), [this](const LightningStrike &strike)
        {
            return (elapsedTimeSec_ - strike.eventTimeSec) > maxAgeSec_;
        }),
        strikes_.end());
}

void Data::InitData()
{

    dataVAO_.create();
    dataVAO_.bind();

    dataVBO_.create();
    dataVBO_.bind();

    dataVAO_.setVertexAttrib(0, 2, 5 * sizeof(GLfloat), 0); // pos
    dataVAO_.setVertexAttrib(1, 1, 5 * sizeof(GLfloat), 2 * sizeof(GLfloat)); // age 
    dataVAO_.setVertexAttrib(2, 1, 5 * sizeof(GLfloat), 3 * sizeof(GLfloat)); // intensity 
    dataVAO_.setVertexAttrib(3, 1, 5 * sizeof(GLfloat), 4 * sizeof(GLfloat)); // polarity

    RefreshVertexData(1280.0f, 720.0f);

    VAO::unbind();
    VBO::unbind();
}

void Data::Update(float deltaTimeSec, float viewportWidth, float viewportHeight)
{
    elapsedTimeSec_ += deltaTimeSec;
    std::size_t importedThisFrame = 0;

    {
        std::lock_guard<std::mutex> lock(queueMutex_);
        while (!strikeQueue_.empty())
        {
            LightningStrike s = std::move(strikeQueue_.front());
            strikeQueue_.pop();
            s.eventTimeSec = elapsedTimeSec_;
            strikes_.push_back(std::move(s));
            ++importedThisFrame;
        }
    }

    static float nextStatsPrintSec = 0.0f;
    if (elapsedTimeSec_ >= nextStatsPrintSec)
    {
        nextStatsPrintSec = elapsedTimeSec_ + 5.0f;
        std::cout << std::fixed << std::setprecision(1)
                  << "Lightning stats t=" << elapsedTimeSec_
                  << "s, imported=" << importedThisFrame
                  << ", active=" << strikes_.size() << std::endl;
    }

    PruneExpiredStrikes();
    RefreshVertexData(viewportWidth, viewportHeight);
}

void Data::DrawPlot()
{
    shader_->use();
    dataVAO_.bind();
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(strikes_.size()));
    VAO::unbind();
}
