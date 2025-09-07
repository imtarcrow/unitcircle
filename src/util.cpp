#include "util.hpp"

#include <iostream>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

namespace util
{

    bool initializeImGui(SDL_Window *window, SDL_Renderer *renderer)
    {
        ImGui::CreateContext();
        ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
        ImGui_ImplSDLRenderer3_Init(renderer);

        // After creating your window and renderer:
        int pixelWidth, pixelHeight;
        SDL_GetWindowSizeInPixels(window, &pixelWidth, &pixelHeight);

        int windowWidth, windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);

        float displayScale = SDL_GetWindowDisplayScale(window);

        ImGuiIO &io = ImGui::GetIO();

        // Set DisplaySize in pixels
        io.DisplaySize = ImVec2((float)pixelWidth, (float)pixelHeight);

        // Set the framebuffer scale (for HiDPI rendering)
        io.DisplayFramebufferScale = ImVec2(displayScale, displayScale);

        ImGui::GetStyle().ScaleAllSizes(0.8f);

        return true;
    }

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