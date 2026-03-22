#ifndef DATA_HPP
#define DATA_HPP

#include <chrono>
#include <memory>
#include <mutex>
#include <queue>
#include <vector>

#include <glm/glm.hpp>

#include "ShaderProgram.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "glad/glad.h"

namespace ix
{
    class WebSocket;
}

struct LightningStrike
{
    float latitude;
    float longitude;
    float intensityNorm;
    float polarity;
    float eventTimeSec;
    float age;
    std::chrono::steady_clock::time_point timestamp;
};

class Data
{
public:
    Data(const ShaderProgram &shader);
    ~Data();

    void DrawPlot();
    void Update(float deltaTimeSec, float viewportWidth, float viewportHeight);

private:
    void InitData();
    void GetBlitzortung();
    void PruneExpiredStrikes();
    void RefreshVertexData(float width, float height);
    static glm::vec2 LatLonToScreen(float lat, float lon, float width, float height);

    std::unique_ptr<ShaderProgram> shader_;
    std::vector<LightningStrike> strikes_;
    std::vector<GLfloat> vertexData_;
    std::queue<LightningStrike> strikeQueue_;

    float elapsedTimeSec_;
    float maxAgeSec_;

    std::mutex queueMutex_;
    std::unique_ptr<ix::WebSocket> websocket_;
    bool websocketStarted_;

    VBO dataVBO_;
    VAO dataVAO_;
};

#endif
