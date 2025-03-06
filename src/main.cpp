#include <SDL3/SDL.h>

#include <iostream>

#define WIDTH 800
#define HEIGHT 800

constexpr float PI = 3.141592653589;
constexpr int POINT_COUNT = 100;
constexpr float LOWER_BOUND = -1;
constexpr float UPPER_BOUND = 1;

struct Vector2
{
    float x;
    float y;
};

bool initializeSDL(SDL_Window **window, SDL_Renderer **renderer)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "Could not initialize" << std::endl;
        *window = nullptr;
        *renderer = nullptr;
        return false;
    }

    SDL_CreateWindowAndRenderer("Kreis", WIDTH, HEIGHT, NULL, window, renderer);

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

struct Vector2 toScreenCoords(struct Vector2 vector, float lower_bound, float upper_bound)
{
    struct Vector2 newVector = {0};

    newVector.x = (vector.x - lower_bound) * (WIDTH / (upper_bound - lower_bound));
    newVector.y = HEIGHT - (vector.y - lower_bound) * (HEIGHT / (upper_bound - lower_bound));

    return newVector;
}

struct Vector2 toBoundCoords(struct Vector2 vector, float lower_bound, float upper_bound)
{
    struct Vector2 newVector = {0};

    newVector.x = vector.x * ((upper_bound - lower_bound) / WIDTH) + lower_bound;
    newVector.y = vector.y * ((upper_bound - lower_bound) / HEIGHT) + lower_bound;

    return newVector;
}

float toBoundCoordSingle(float coord, float lower_bound, float upper_bound)
{
    return coord * ((upper_bound - lower_bound) / WIDTH) + lower_bound;
}

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
    float b = 2.0f;
    float c = 1.0f;
    float d = 0.0f;

    return a * SDL_cosf(b * (v - c)) + d;
}

SDL_FPoint *generatePoints()
{

    SDL_FPoint *points = static_cast<SDL_FPoint *>(calloc(POINT_COUNT + 1, sizeof(SDL_FPoint)));

    for (int i = 0; i < POINT_COUNT; i++)
    {

        float positionOnCircle = ((2 * PI) / POINT_COUNT) * i;

        std::cout << positionOnCircle << std::endl;

        struct Vector2 position;

        position.x = custom_cosine(positionOnCircle);
        position.y = custom_sine(positionOnCircle);

        struct Vector2 screenspacePosition = toScreenCoords(position, LOWER_BOUND, UPPER_BOUND);

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

    if (!initializeSDL(&window, &renderer))
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
        for (int i = 0; i < POINT_COUNT - 1; i++)
        {
            SDL_FPoint p = points[i];
            SDL_FPoint next_p;
            if (i == POINT_COUNT - 1)
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

    uninitializeSDL(&window, &renderer);

    SDL_Quit();
    return 0;
}
