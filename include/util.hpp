#pragma once

#include <SDL3/SDL.h>

namespace util
{
    constexpr float PI = 3.141592653589;

    struct Properties
    {
        float a;
        float b;
        float c;
        float d;
    };

    struct Vector2
    {
        float x;
        float y;
    };

    bool initializeImGui(SDL_Window *window, SDL_Renderer *renderer);
    bool initializeSDL(SDL_Window **window, SDL_Renderer **renderer, int width, int height);
    void uninitializeSDL(SDL_Window **window, SDL_Renderer **renderer);
    struct Vector2 boundToScreenspace(struct Vector2 vector, float width, float height, float scaling);
    float toBoundCoord(float coord, float width, float scaling);
}
