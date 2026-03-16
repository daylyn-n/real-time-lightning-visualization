#ifndef INPUT_HPP
#define INPUT_HPP

#include <SDL2/SDL.h>
#include "Camera.hpp"

class Input
{
    public:
        Input();

        void processEvents(SDL_Window* window, Camera& camera);
        bool shouldQuit() const;

    private:
        bool quitRequested_;
        int mouseX_;
        int mouseY_;
};

#endif