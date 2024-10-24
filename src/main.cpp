#include <iostream>
#include <sdl.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "math.hpp"
#include "Ray.hpp"
#include "../Sphere.hpp"

using Color = Vector3;

// Window
const double ASPECT_RATIO = 16.0/9.0;
const int IMAGE_WIDTH = 400;
const int IMAGE_HEIGHT = IMAGE_WIDTH / ASPECT_RATIO;

// Camera
const double VIEWPORT_HEIGHT = 2.0;
const double VIEWPORT_WIDTH = VIEWPORT_HEIGHT * (static_cast<double>(IMAGE_WIDTH) / IMAGE_HEIGHT);
const double FOCAL_LENGTH = 1.0;
const Vector3 CAMERA_CENTER{0, 0, 0};

const Vector3 VIEWPORT_POSITIVE_X{VIEWPORT_WIDTH, 0, 0};
const Vector3 VIEWPORT_POSITIVE_Y{0, -VIEWPORT_HEIGHT, 0};

const Vector3 PIXEL_DELTA_X = VIEWPORT_POSITIVE_X / IMAGE_WIDTH;
const Vector3 PIXEL_DELTA_Y = VIEWPORT_POSITIVE_Y / IMAGE_HEIGHT;

const Vector3 VIEWPORT_UPPER_LEFT = CAMERA_CENTER - Vector3{0, 0, FOCAL_LENGTH} - VIEWPORT_POSITIVE_X/2 - VIEWPORT_POSITIVE_Y/2;
const Vector3 ORIGIN_PIXEL_LOCATION = VIEWPORT_UPPER_LEFT + 0.5 * (PIXEL_DELTA_X + PIXEL_DELTA_Y);

void drawUI()
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // ImGui::ShowDemoWindow();
}

const Sphere sphere{{0, 0, -1}, 0.5};

Color rayColor(const Ray& ray)
{
    if (sphere.intersects(ray))
    {
        return Color{0, 1, 1};
    }

    auto a = 0.5*(ray.direction.normalised().y + 1.0);
    return (1.0-a)*Color(1.0, 1.0, 1.0) + a*Color(0.5, 0.7, 1.0);
}

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("Ray Tracing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, IMAGE_WIDTH, IMAGE_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    if (window == nullptr)
    {
        std::cout << "Failed to initialise SDL window: " << SDL_GetError() << "\n";
        return 1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    const ImGuiIO& io = ImGui::GetIO(); (void)io;

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

        for (int i = 0; i < windowWidth; i++)
        {
            for (int j = 0; j < windowHeight; j++)
            {
                auto pixel_center = ORIGIN_PIXEL_LOCATION + (i * PIXEL_DELTA_X) + (j * PIXEL_DELTA_Y);
                auto ray_direction = pixel_center - CAMERA_CENTER;
                Ray r(CAMERA_CENTER, ray_direction);
                Color c = rayColor(r);

                SDL_SetRenderDrawColor(renderer, c.x * 255.0, c.y * 255.0, c.z*255.0, 255);
                SDL_RenderDrawPoint(renderer, i, j);
            }
        }

        drawUI();

        ImGui::Render();
        SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);

        SDL_RenderPresent(renderer);
    }

    return 0;
}
