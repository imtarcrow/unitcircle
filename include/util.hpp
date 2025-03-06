#pragma once

#include <SDL3/SDL.h>

namespace util
{
    constexpr float PI = 3.141592653589;

    struct Vector2
    {
        float x;
        float y;
    };

    bool initializeSDL(SDL_Window **window, SDL_Renderer **renderer, int width, int height);
    void uninitializeSDL(SDL_Window **window, SDL_Renderer **renderer);
    struct Vector2 boundToScreenspace(struct Vector2 vector, float width, float height, float scaling);
    float toBoundCoord(float coord, float width, float scaling);
}
