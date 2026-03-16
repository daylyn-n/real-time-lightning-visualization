#include "../include/Input.hpp"

Input::Input()
    : quitRequested_(false),
      mouseX_(0),
      mouseY_(0)
{
}

void Input::processEvents(SDL_Window* window, Camera& camera)
{
    (void)window;
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
        {
            quitRequested_ = true;
        }

        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
        {
            quitRequested_ = true;
        }

        if (event.type == SDL_MOUSEMOTION)
        {
            mouseX_ += event.motion.xrel;
            mouseY_ += event.motion.yrel;
            camera.MouseLook(mouseX_ - 1, mouseY_ - 1);
        }
    }

    const Uint8* state = SDL_GetKeyboardState(nullptr);
    if (state[SDL_SCANCODE_W])
    {
        camera.MoveForward(camera.speed);
    }
    if (state[SDL_SCANCODE_S])
    {
        camera.MoveBackward(camera.speed);
    }
    if (state[SDL_SCANCODE_D])
    {
        camera.MoveRight(camera.speed);
    }
    if (state[SDL_SCANCODE_A])
    {
        camera.MoveLeft(camera.speed);
    }
    if (state[SDL_SCANCODE_SPACE])
    {
        camera.MoveUp(camera.speed - 0.02f);
    }
    if (state[SDL_SCANCODE_C])
    {
        camera.MoveDown(camera.speed - 0.02f);
    }
}

bool Input::shouldQuit() const
{
    return quitRequested_;
}
