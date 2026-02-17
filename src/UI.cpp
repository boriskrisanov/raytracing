#include "UI.hpp"

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

#include "Renderer.hpp"

UI::UI(SDL_Window* window, SDL_Renderer* sdlRenderer, Renderer& renderer, Camera& camera)
    : window(window), sampleCount(1), bounceCount(1), sdlRenderer(sdlRenderer), renderer(renderer), camera(camera)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();

    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window, sdlRenderer);
    ImGui_ImplSDLRenderer2_Init(sdlRenderer);
}

void UI::update()
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // Draw interface
    ImGui::Begin("Ray Tracer");

    const int lastSampleCount = sampleCount;
    const int lastBounceCount = bounceCount;

    ImGui::InputInt("Samples", &sampleCount, 1, 32);
    ImGui::InputInt("Bounces", &bounceCount, 1, 4);

    sampleCount = std::max(sampleCount, 1);
    bounceCount = std::max(bounceCount, 1);

    if ((sampleCount != lastSampleCount || bounceCount != lastBounceCount))
    {
        if (renderer.isRenderInProgress())
        {
            renderer.stopRender();
        }
        renderer.startRenderAsync(sampleCount, bounceCount);
    }

    // const auto cameraPosPointers = {};
    // ImGui::SliderFloat3("Camera position", )

    ImGui::End();
    // ImGui::ShowDemoWindow();

    ImGui::Render();
    SDL_RenderSetScale(sdlRenderer, io->DisplayFramebufferScale.x, io->DisplayFramebufferScale.y);
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), sdlRenderer);
}
