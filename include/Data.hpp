#ifndef DATA_HPP
#define DATA_HPP

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#include "ShaderProgram.hpp"
#include "Texture.hpp"
#include "glad/glad.h"
#include "VBO.hpp"
#include "VAO.hpp"
#include "MapRenderer.hpp"
#include <vector>
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>

// threading for websockets to get lughtning data


struct LightningStrike
{
    float latitude;
    float longitude;
    float eventTimeSec;
    float intensityNorm;
    float polarity;
};

class Data
{
    public:
        Data(const ShaderProgram &shader);
        ~Data();
        void DrawPlot();
        void Update(float deltaTimeSec, float viewportWidth, float viewportHeight);
    private:
        std::unique_ptr<ShaderProgram> shader_;
        std::vector<LightningStrike> strikes_;
        std::vector<GLfloat> vertexData_;
        float elapsedTimeSec_;
        float maxAgeSec_;
        VBO dataVBO_;
        VAO dataVAO_;
        void InitData();
        static glm::vec2 LatLonToScreen(float lat, float lon,
                                        float width,
                                        float height);
        void RefreshVertexData(float width, float height);

        std::vector<LightningStrike> GetCSVData(const std::string &csv);
};

#endif
