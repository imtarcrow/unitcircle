#include <SDL3/SDL.h>

#include "util.hpp"

#include <iostream>

constexpr int WIDTH = 800;
constexpr int HEIGHT = 800;

constexpr int SAMPLES = 1000;
constexpr float SCALING = 1.0f;

float custom_sine(float v)
{
    float a = 1.0f;
    float b = 1.0f;
    float c = 0.0f;
    float d = 0.0f;

    return a * SDL_sinf(b * (v - c)) + d;
}

float custom_cosine(float v)
{
    float a = 1.0f;
    float b = 1.1f;
    float c = 0.0f;
    float d = 0.0f;

    return a * SDL_cosf(b * (v - c)) + d;
}

SDL_FPoint *generatePoints()
{

    SDL_FPoint *points = static_cast<SDL_FPoint *>(calloc(SAMPLES + 1, sizeof(SDL_FPoint)));

    for (int i = 0; i < SAMPLES; i++)
    {

        float positionOnCircle = ((2 * util::PI) / SAMPLES) * i;

        std::cout << positionOnCircle << std::endl;

        struct util::Vector2 position;

        position.x = custom_cosine(positionOnCircle);
        position.y = custom_sine(positionOnCircle);

        struct util::Vector2 screenspacePosition = util::boundToScreenspace(position, static_cast<float>(WIDTH), static_cast<float>(HEIGHT), SCALING);

        SDL_FPoint p = {
            screenspacePosition.x,
            screenspacePosition.y,
        };

        points[i] = p;
    }

    return points;
}

int main()
{

    SDL_Window *window;
    SDL_Renderer *renderer;

    if (!util::initializeSDL(&window, &renderer, WIDTH, HEIGHT))
    {
        return 1;
    }

    bool running = true;
    while (running)
    {

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        // Draw coordinate system
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
        SDL_RenderLine(renderer, WIDTH / 2, 0, WIDTH / 2, HEIGHT);
        SDL_RenderLine(renderer, 0, HEIGHT / 2, WIDTH, HEIGHT / 2);

        SDL_FPoint *points = generatePoints();

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
        for (int i = 0; i < SAMPLES - 1; i++)
        {
            SDL_FPoint p = points[i];
            SDL_FPoint next_p;
            if (i == SAMPLES - 1)
            {
                next_p = points[i + 1];
            }
            else
            {
                next_p = points[i + 1];
            }

            SDL_RenderLine(renderer, p.x, p.y, next_p.x, next_p.y);
        }

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
        // SDL_RenderPoints(renderer, points, POINT_COUNT);

        SDL_RenderPresent(renderer);
    }

    util::uninitializeSDL(&window, &renderer);

    SDL_Quit();
    return 0;
}
