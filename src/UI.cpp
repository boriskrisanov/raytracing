#include "UI.hpp"

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

#include "Camera.hpp"
#include "Renderer.hpp"

UI::UI(SDL_Window* window, SDL_Renderer* sdlRenderer, Renderer& renderer, Camera& camera)
    : window(window), sdlRenderer(sdlRenderer), renderer(renderer), camera(camera), sampleCount(1000), bounceCount(5)
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

    bool shouldRestartRender = false;

    // Draw interface
    ImGui::Begin("Options");

    const int lastSampleCount = sampleCount;
    const int lastBounceCount = bounceCount;

    ImGui::InputInt("Samples", &sampleCount, 1, 32);
    ImGui::InputInt("Bounces", &bounceCount, 1, 4);

    sampleCount = std::max(sampleCount, 1);
    bounceCount = std::max(bounceCount, 1);

    if (sampleCount != lastSampleCount || bounceCount != lastBounceCount)
    {
        shouldRestartRender = true;
    }

    const bool lastShadeNormals = renderer.shadeNormals;
    ImGui::Checkbox("Shade normals", &renderer.shadeNormals);
    if (renderer.shadeNormals != lastShadeNormals)
    {
        shouldRestartRender = true;
    }

    const bool lastShadeBoundingBoxes = renderer.shadeBoundingBoxes;
    ImGui::Checkbox("Draw bounding boxes", &renderer.shadeBoundingBoxes);
    if (renderer.shadeBoundingBoxes != lastShadeBoundingBoxes)
    {
        shouldRestartRender = true;
    }

    ImGui::SliderFloat3("Camera position", cameraPosition.data(), -5, 5);

    if (!fp_utils::equals(cameraPosition[0], camera.position.x) ||
        !fp_utils::equals(cameraPosition[1], camera.position.y) ||
        !fp_utils::equals(cameraPosition[2], camera.position.z))
    {
        shouldRestartRender = true;
    }

    camera.position.x = cameraPosition[0];
    camera.position.y = cameraPosition[1];
    camera.position.z = cameraPosition[2];

    ImGui::SliderFloat3("Camera rotation", cameraRotation.data(), -180, 180);

    if (!fp_utils::equals(cameraRotation[0], camera.rotation.x) ||
        !fp_utils::equals(cameraRotation[1], camera.rotation.y) ||
        !fp_utils::equals(cameraRotation[2], camera.rotation.z))
    {
        shouldRestartRender = true;
    }

    camera.rotation.x = cameraRotation[0];
    camera.rotation.y = cameraRotation[1];
    camera.rotation.z = cameraRotation[2];


    if (shouldRestartRender)
    {
        if (renderer.isRenderInProgress())
        {
            renderer.stopRender();
        }
        renderer.startRenderAsync(sampleCount, bounceCount);
    }

    ImGui::Text("Samples: %i/%i", static_cast<int>(renderer.getCompletedSampleCount()), sampleCount);

    ImGui::End();
    // ImGui::ShowDemoWindow();

    ImGui::Render();
    SDL_RenderSetScale(sdlRenderer, io->DisplayFramebufferScale.x, io->DisplayFramebufferScale.y);
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), sdlRenderer);
}
