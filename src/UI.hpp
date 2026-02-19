#pragma once

#include <array>
#include <SDL_render.h>
#include <SDL_video.h>

class Camera;
class Renderer;
struct ImGuiIO;

class UI
{
public:
    UI(SDL_Window* window, SDL_Renderer* sdlRenderer, Renderer& renderer, Camera& camera);
    void update();
private:
    SDL_Window* window;
    SDL_Renderer* sdlRenderer;
    Renderer& renderer;
    Camera& camera;
    ImGuiIO* io;

    // UI options

    int sampleCount;
    int bounceCount;
    std::array<float, 3> cameraPosition{0, 0, 0};
};
