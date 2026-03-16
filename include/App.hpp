#ifndef APP_HPP
#define APP_HPP

#include "Camera.hpp"
#include "Input.hpp"
#include "Window.hpp"
#include "VBO.hpp"
#include "VAO.hpp"
#include "ShaderProgram.hpp"
#include "Texture.hpp"
#include <memory>
#include "MapRenderer.hpp"
class App
{
    public:
        App();

        bool initialize();
        void run();
        void shutdown();

    private:
        bool setupTriangle();
        void destroyTriangle();
        void update(float deltaTime);
        void render();

        Window window_;
        Input input_;
        Camera camera_;
        VAO triangleVao_;
        VBO triangleVbo_;
        std::unique_ptr<ShaderProgram> triangleShader_;
        bool initialized_;
        MapRenderer *map_;
        Texture2D mapTexture_;
};

#endif
