#include "util.hpp"

#include <iostream>

namespace util
{

    bool initializeSDL(SDL_Window **window, SDL_Renderer **renderer, int width, int height)
    {
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            std::cout << "Could not initialize" << std::endl;
            *window = nullptr;
            *renderer = nullptr;
            return false;
        }

        SDL_CreateWindowAndRenderer("Kreis", width, height, 0, window, renderer);

        if (*window == NULL || *renderer == NULL)
        {
            std::cout << "Failed to create window or renderer via SDL! Code: " << SDL_GetError() << std::endl;
            *window = nullptr;
            *renderer = nullptr;
            return false;
        }

        return true;
    }
    void uninitializeSDL(SDL_Window **window, SDL_Renderer **renderer)
    {
        if (*renderer != nullptr)
        {
            SDL_DestroyRenderer(*renderer);
            *renderer = nullptr;
        }

        if (*window != nullptr)
        {
            SDL_DestroyWindow(*window);
            *window = nullptr;
        }
    }

    struct Vector2 boundToScreenspace(struct Vector2 vector, float width, float height, float scaling)
    {
        struct Vector2 newVector = {0};

        // x = y * (r + value) / (2 * r)

        newVector.x = width * (scaling + vector.x) / (2 * scaling);
        newVector.y = height - (height * (scaling + vector.y) / (2 * scaling));

        return newVector;
    }

    float toBoundCoord(float coord, float dimension, float scaling)
    {
        return -scaling + ((2 * scaling * coord) / dimension);
    }

}