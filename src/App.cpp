#include "../include/App.hpp"
#include "../include/ResourceManager.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

#include "../thirdparty/glm-master/glm/glm.hpp"
#include "../thirdparty/glm-master/glm/vec3.hpp"
#include "../thirdparty/glm-master/glm/vec4.hpp"
#include "../thirdparty/glm-master/glm/mat4x4.hpp"
#include "../thirdparty/glm-master/glm/ext/matrix_clip_space.hpp"
#include "../thirdparty/glm-master/glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "../thirdparty/glm-master/glm/gtx/rotate_vector.hpp"

App::App()
    : window_(1280, 720, "Realtime Geographic Visualizer"),
    initialized_(false),
    map_(nullptr)
{
}

bool App::initialize()
{
    if (!window_.create())
    {
        std::cout << "Failed to initialize application window" << std::endl;
        return false;
    }

    // load shaders and textures
    ResourceManager::LoadShader("shaders/map_vert.glsl", "shaders/map_frag.glsl",
                    "map");
    glm::mat4 projection = glm::ortho(0.0f, (float)window_.width(),
                        (float)window_.height(), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("map").use();
    ResourceManager::GetShader("map").setInt("uImage", 0);
    ResourceManager::GetShader("map").setMat4("uProjection", projection);

    map_ = new MapRenderer(ResourceManager::GetShader("map"));
    mapTexture_ = ResourceManager::LoadTexture("assets/map.png", true, "mapTex");
    

    SDL_SetRelativeMouseMode(SDL_TRUE); 
    initialized_ = true;
    return true;
}

void App::run()
{
    if (!initialized_)
    {
        return;
    }

    Uint32 lastTicks = SDL_GetTicks();
    while (!input_.shouldQuit())
    {
        Uint32 now = SDL_GetTicks();
        float deltaTime = static_cast<float>(now - lastTicks) / 1000.0f;
        lastTicks = now;

        input_.processEvents(window_.nativeHandle(), camera_);
        update(deltaTime);
        render();
        window_.swapBuffers();
    }
}

void App::shutdown()
{
    if (!initialized_)
    {
        return;
    }

    destroyTriangle();
    delete map_;
    map_ = nullptr;
    ResourceManager::Clear();
    window_.destroy();
    initialized_ = false;
}

bool App::setupTriangle()
{
    const std::vector<GLfloat> vertexPos
    {
        -0.8f, -0.8f, 0.0f,
         0.8f, -0.8f, 0.0f,
         0.0f,  0.8f, 0.0f,
    };

    triangleVao_.create();
    triangleVao_.bind();

    triangleVbo_.create();
    triangleVbo_.bind();
    triangleVbo_.setData(vertexPos);

    triangleVao_.setVertexAttrib(0, 3, 3 * sizeof(GLfloat), 0);
    VBO::unbind();
    VAO::unbind();

    triangleShader_ = std::make_unique<ShaderProgram>(
        "shaders/simple_triangle_vert.glsl",
        "shaders/simple_triangle_frag.glsl");

    if (triangleShader_->GraphicsPipelineShaderProgram == 0)
    {
        return false;
    }

    return true;
}

void App::destroyTriangle()
{
    triangleShader_.reset();
    triangleVbo_.destroy();
    triangleVao_.destroy();
}

void App::update(float deltaTime)
{
    (void)deltaTime;
    
}

void App::render()
{
    window_.setClearColor(0.03f, 0.06f, 0.10f, 1.0f);
    window_.clear();

    if (map_ != nullptr)
    {
        map_->DrawMap(mapTexture_, glm::vec2(0.0f, 0.0f),
                    glm::vec2((float)window_.width(), (float)window_.height()));
    }

    VAO::unbind();
}
