#include "../include/App.hpp"
#include "../include/ResourceManager.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

App::App()
    : window_(1280, 720, "Realtime Geographic Visualizer"),
    initialized_(false),
    map_(nullptr),
    lightningData_(nullptr)
{
}

bool App::initialize()
{
    if (!window_.create())
    {
        std::cout << "Failed to initialize application window" << std::endl;
        return false;
    }

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // load shaders and textures
    ResourceManager::LoadShader("shaders/map_vert.glsl", "shaders/map_frag.glsl",
                    "map");
    ResourceManager::LoadShader("shaders/lightning_vert.glsl",
                                "shaders/lightning_frag.glsl",
                                "lightning");
    glm::mat4 projection = glm::ortho(0.0f, (float)window_.width(),
                        (float)window_.height(), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("map").use();
    ResourceManager::GetShader("map").setInt("uImage", 0);
    ResourceManager::GetShader("map").setMat4("uProjection", projection);

    ResourceManager::GetShader("lightning").use();
    ResourceManager::GetShader("lightning").setMat4("uProjection", projection);
    ResourceManager::GetShader("lightning").setFloat("uBasePointSize", 8.0f);
    ResourceManager::GetShader("lightning").setFloat("uSizeScale", 5.0f);

    map_ = std::make_unique<MapRenderer>(ResourceManager::GetShader("map"));
    lightningData_ = std::make_unique<Data>(ResourceManager::GetShader("lightning"));
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

    lightningData_.reset();
    map_.reset();
    ResourceManager::Clear();
    window_.destroy();
    initialized_ = false;
}


void App::update(float deltaTime)
{
    if (lightningData_ != nullptr)
    {
        lightningData_->Update(deltaTime,
                            static_cast<float>(window_.width()),
                            static_cast<float>(window_.height()));
    }
}

void App::render()
{
    window_.setClearColor(0.03f, 0.06f, 0.10f, 1.0f);
    window_.clear();

    glDisable(GL_DEPTH_TEST);

    if (map_ != nullptr)
    {
        map_->DrawMap(mapTexture_, glm::vec2(0.0f, 0.0f),
                    glm::vec2((float)window_.width(), (float)window_.height()));
    }

    
    if (lightningData_ != nullptr)
    {
        lightningData_->DrawPlot();
    }

    VAO::unbind();
}
