#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL2/SDL.h>
#include <string>

class Window
{
    public:
        Window(int width, int height, std::string title);
        ~Window();

        bool create();
        void destroy();

        void swapBuffers() const;
        void setClearColor(float r, float g, float b, float a);
        void clear() const;

        bool isValid() const;
        SDL_Window* nativeHandle() const;

        int width() const;
        int height() const;

    private:
        int width_;
        int height_;
        std::string title_;

        SDL_Window* window_;
        SDL_GLContext glContext_;
};

#endif