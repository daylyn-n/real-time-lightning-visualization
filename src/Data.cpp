#include "../include/Data.hpp"
#include <algorithm>
#include <cstddef>
#include <fstream>
#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
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

std::vector<LightningStrike> Data::GetCSVData(const std::string& csv)
{
    std::ifstream file(csv);
    if(!file.is_open())
    {
        std::cerr << "Error opening file" << std::endl;
        exit(1);
    }
    std::string line;

    // loops every new line
    while(std::getline(file, line, '\n'))
    {
        std::stringstream ss(line);
        std::string value;

        std::vector<float> values;

        while(std::getline(ss, value, ','))
        {
            values.push_back(std::stof(value));
        }

        if(values.size() >= 5)
        {
            strikes_.push_back({
                    values[0],
                    values[1],
                    values[2],
                    values[3],
                    values[4],
                    });
        }
    }
    return strikes_;
}
void Data::InitData()
{
    GetCSVData("assets/test_data.csv");

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
    RefreshVertexData(viewportWidth, viewportHeight);
}

void Data::DrawPlot()
{
    shader_->use();
    dataVAO_.bind();
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(strikes_.size()));
    VAO::unbind();
}











