#include <iostream>
#include <sdl.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

void drawUI()
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();
}

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("Ray Tracing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    if (window == nullptr)
    {
        std::cout << "Failed to initialise SDL window: " << SDL_GetError() << "\n";
        return 1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    while (true)
    {
        int windowWidth = 0;
        int windowHeight = 0;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
        SDL_Event event;
        SDL_PollEvent(&event);

        if (event.type == SDL_QUIT)
        {
            break;
        }

        ImGui_ImplSDL2_ProcessEvent(&event);

        SDL_RenderClear(renderer);

        // for (int i = 0; i < windowWidth; i++)
        // {
        //     for (int j = 0; j < windowHeight; j++)
        //     {
        //         SDL_SetRenderDrawColor(renderer, i, j, 255, 255);
        //         SDL_RenderDrawPoint(renderer, i, j);
        //     }
        // }

        drawUI();

        ImGui::Render();
        SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);

        SDL_RenderPresent(renderer);
    }

    return 0;
}
