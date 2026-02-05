#pragma once

#include <SDL_render.h>
#include <SDL_video.h>

struct ImGuiIO;

class UI
{
public:
    UI(SDL_Window* window, SDL_Renderer* renderer);
    void update() const;
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    ImGuiIO* io;
};
