#include "../include/Data.hpp"
#include <algorithm>
#include <memory>
#include <vector>

Data::Data(const ShaderProgram &shader)
    : elapsedTimeSec_(0.0f),
    maxAgeSec_(25.0f)
{
    shader_ = std::make_unique<ShaderProgram>(shader);
    InitData();
}

Data::~Data()
{
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

void Data::InitData()
{
    strikes_ = {
        {0.0f, 0.0f, 0.5f, 0.80f, 1.0f},
        {51.5072f, -0.1276f, 2.0f, 0.70f, -1.0f},
        {40.7128f, -74.0060f, 3.2f, 0.62f, 1.0f},
        {35.6762f, 139.6503f, 5.0f, 0.55f, -1.0f},
        {-33.8688f, 151.2093f, 7.0f, 0.95f, 1.0f},
        {-23.5505f, -46.6333f, 8.5f, 0.74f, -1.0f},
        {64.2008f, -149.4937f, 10.0f, 0.48f, 1.0f},
        {-77.8419f, 166.6863f, 11.5f, 0.66f, -1.0f}
    };

    dataVAO_.create();
    dataVAO_.bind();

    dataVBO_.create();
    dataVBO_.bind();

    dataVAO_.setVertexAttrib(0, 2, 5 * sizeof(GLfloat), 0);
    dataVAO_.setVertexAttrib(1, 1, 5 * sizeof(GLfloat), 2 * sizeof(GLfloat));
    dataVAO_.setVertexAttrib(2, 1, 5 * sizeof(GLfloat), 3 * sizeof(GLfloat));
    dataVAO_.setVertexAttrib(3, 1, 5 * sizeof(GLfloat), 4 * sizeof(GLfloat));

    RefreshVertexData(1280.0f, 720.0f);

    VAO::unbind();
    VBO::unbind();
}

void Data::Update(float deltaTimeSec, float viewportWidth, float viewportHeight)
{
    elapsedTimeSec_ += deltaTimeSec;
    RefreshVertexData(viewportWidth, viewportHeight);
}

void Data::DrawPlot()
{
    shader_->use();
    dataVAO_.bind();
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(strikes_.size()));
    VAO::unbind();
}
