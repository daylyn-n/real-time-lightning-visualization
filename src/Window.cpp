#include "../include/Window.hpp"

#include "../include/glad/glad.h"
#include <iostream>
#include <utility>

Window::Window(int width, int height, std::string title)
    : width_(width),
      height_(height),
      title_(std::move(title)),
      window_(nullptr),
      glContext_(nullptr)
{
}

Window::~Window()
{
    destroy();
}

bool Window::create()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize video: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window_ = SDL_CreateWindow(
        title_.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width_,
        height_,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (window_ == nullptr)
    {
        std::cout << "SDL window creation failed: " << SDL_GetError() << std::endl;
        return false;
    }

    glContext_ = SDL_GL_CreateContext(window_);
    if (glContext_ == nullptr)
    {
        std::cout << "OpenGL context creation failed: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        std::cout << "GLAD initialization failed" << std::endl;
        return false;
    }

    SDL_GL_SetSwapInterval(1);
    glViewport(0, 0, width_, height_);

    return true;
}

void Window::destroy()
{
    if (glContext_ != nullptr)
    {
        SDL_GL_DeleteContext(glContext_);
        glContext_ = nullptr;
    }

    if (window_ != nullptr)
    {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }

    SDL_Quit();
}

void Window::swapBuffers() const
{
    if (window_ != nullptr)
    {
        SDL_GL_SwapWindow(window_);
    }
}

void Window::setClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void Window::clear() const
{
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Window::isValid() const
{
    return window_ != nullptr && glContext_ != nullptr;
}

SDL_Window* Window::nativeHandle() const
{
    return window_;
}

int Window::width() const
{
    return width_;
}

int Window::height() const
{
    return height_;
}
