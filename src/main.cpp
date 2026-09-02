#include <SDL3/SDL.h>

#include "util.hpp"

#include <imgui.h>

#include <iostream>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
constexpr int WIDTH = 800;
constexpr int HEIGHT = 800;

float custom_sine(float v, struct util::Properties props)
{

    return props.a * SDL_sinf(props.b * (v - props.c)) + props.d;
}

float custom_cosine(float v, struct util::Properties props)
{
    return props.a * SDL_cosf(props.b * (v - props.c)) + props.d;
}

SDL_FPoint *generatePoints(int samples, float scaling, struct util::Properties sine_properties, struct util::Properties cosine_properties)
{

    SDL_FPoint *points = static_cast<SDL_FPoint *>(malloc(sizeof(SDL_FPoint) * samples));

    for (int i = 0; i < samples; i++)
    {

        float positionOnCircle = ((2 * util::PI) / samples) * i;

        struct util::Vector2 position;

        position.x = custom_cosine(positionOnCircle, cosine_properties);
        position.y = custom_sine(positionOnCircle, sine_properties);

        struct util::Vector2 screenspacePosition = util::boundToScreenspace(position, static_cast<float>(WIDTH), static_cast<float>(HEIGHT), scaling);

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

    bool connectLast = false;
    float scaling = 2.0f;
    int samples = 100;

    struct util::Properties sine_properties;
    struct util::Properties cosine_properties;

    float graphColor[3] = {0, 0, 255};
    float functionColor[3] = {0, 255, 0};
    float pointColor[3] = {255, 0, 0};

    sine_properties.a = 1.0f;
    sine_properties.b = 1.0f;
    sine_properties.c = 0.0f;
    sine_properties.d = 0.0f;

    cosine_properties.a = 1.0f;
    cosine_properties.b = 1.0f;
    cosine_properties.c = 0.0f;
    cosine_properties.d = 0.0f;

    SDL_Window *window;
    SDL_Renderer *renderer;

    if (!util::initializeSDL(&window, &renderer, WIDTH, HEIGHT))
    {
        return 1;
    }

    if (!util::initializeImGui(window, renderer))
    {
        return 1;
    }

    bool running = true;
    while (running)
    {

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);

            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        // Draw coordinate system
        SDL_SetRenderDrawColorFloat(renderer, graphColor[0], graphColor[1], graphColor[2], 0);
        SDL_RenderLine(renderer, WIDTH / 2, 0, WIDTH / 2, HEIGHT);
        SDL_RenderLine(renderer, 0, HEIGHT / 2, WIDTH, HEIGHT / 2);

        SDL_FPoint *points = generatePoints(samples, scaling, sine_properties, cosine_properties);

        SDL_SetRenderDrawColorFloat(renderer, functionColor[0], functionColor[1], functionColor[2], 0);

        if (connectLast)
        {
            for (int i = 0; i < samples; i++)
            {
                SDL_FPoint p = points[i];
                SDL_FPoint np;

                if (i == samples - 1)
                {
                    np = points[0];
                }
                else
                {
                    np = points[i + 1];
                }

                SDL_RenderLine(renderer, p.x, p.y, np.x, np.y);
            }
        }
        else
        {
            for (int i = 0; i < samples - 1; i++)
            {
                SDL_FPoint p = points[i];
                SDL_FPoint np = points[i + 1];

                SDL_RenderLine(renderer, p.x, p.y, np.x, np.y);
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
        // SDL_RenderPoints(renderer, points, POINT_COUNT);

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Settings");
        ImGui::Text("Sinus");
        ImGui::DragFloat("(A) Amplitude##sine", &sine_properties.a, 0.01f, -10.0f, 10.0f);
        ImGui::DragFloat("(B) Perioden##sine", &sine_properties.b, 0.01f, -10.0f, 10.0f);
        ImGui::DragFloat("(C) X Achsenverschiebung##sine", &sine_properties.c, 0.01f, -10.0f, 10.0f);
        ImGui::DragFloat("(D) Y Achsenverschiebung##sine", &sine_properties.d, 0.01f, -10.0f, 10.0f);
        ImGui::Separator();
        ImGui::Text("Kosinus");
        ImGui::DragFloat("(A) Amplitude##cosine", &cosine_properties.a, 0.01f, -10.0f, 10.0f);
        ImGui::DragFloat("(B) Perioden##cosine", &cosine_properties.b, 0.01f, -10.0f, 10.0f);
        ImGui::DragFloat("(C) X Achsenverschiebung##cosine", &cosine_properties.c, 0.01f, -10.0f, 10.0f);
        ImGui::DragFloat("(D) Y Achsenverschiebung##cosine", &cosine_properties.d, 0.01f, -10.0f, 10.0f);
        ImGui::Separator();

        ImGui::DragFloat("Scaling", &scaling, 0.01f, 0.5f, 10.0f);
        ImGui::DragInt("Samples", &samples, 1, 1, 10000);

        ImGui::Checkbox("Ersten und Letzen verbinden", &connectLast);

        ImGui::End();

        ImGui::Begin("Farben");

        ImGui::ColorPicker3("Funktionsfarbe", functionColor);
        ImGui::ColorPicker3("Graphfarbe", graphColor);
        ImGui::ColorPicker3("Punktfarbe", pointColor);

        ImGui::End();
        // ImGui::ShowDemoWindow();

        ImGui::Render();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);

        HEIGHT / 2

        free(points);
    }

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    util::uninitializeSDL(&window, &renderer);

    SDL_Quit();
    return 0;
}
