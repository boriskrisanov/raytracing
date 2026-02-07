#include <array>
#include <iostream>
#include <sdl.h>

#include "SceneObject.hpp"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "math.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include <vector>

#include "Camera.hpp"
#include "Diffuse.hpp"
#include "Emissive.hpp"
#include "Reflective.hpp"
#include "UI.hpp"
#include "Renderer.hpp"
#include "Triangle.hpp"

using Color = Vector3;

// Window
constexpr double ASPECT_RATIO = 16.0 / 9.0;
constexpr int IMAGE_WIDTH = 800;
constexpr int IMAGE_HEIGHT = IMAGE_WIDTH / ASPECT_RATIO;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("Ray Tracing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, IMAGE_WIDTH,
                                          IMAGE_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    SDL_Renderer* sdlRenderer = SDL_CreateRenderer(window, -1, 0);

    if (window == nullptr)
    {
        std::cout << "Failed to initialise SDL window: " << SDL_GetError() << "\n";
        return 1;
    }

    std::vector<SceneObject*> sceneObjects;

    auto* m1 = new Diffuse{Vector3{1, 1, 1}};
    auto* m2 = new Diffuse{Vector3{1, 1, 1}};
    auto* metal = new Reflective{Vector3{0.5, 0.5, 1}, 0.1};
    auto* roughMetal = new Reflective{Vector3{0.5, 1, 1}, 0.5};
    auto* light = new Emissive{Vector3{1.0, 1.0, 1.0},  1};

    // sceneObjects.push_back(new Sphere{Vector3{0, 0, -1}, 0.5, m1});
    // sceneObjects.push_back(new Sphere{Vector3{-1, 0, -1}, 0.5, metal});
    // sceneObjects.push_back(new Sphere{Vector3{1, 0, -1}, 0.5, roughMetal});


    // sceneObjects.push_back(new Sphere{Vector3{0, 6, -0.5}, 5, light});
    // sceneObjects.push_back(new Sphere{Vector3{0, -100.5, 0}, 100, m2});

    auto* t = new Triangle{{-1, 0, -1}, {1, 0, -1}, {0, 1, -1}, m1};
    std::cout << t->intersects(Ray({0, 0.5, 0}, {0, 0, -1}), Interval(0.0001, 100)).didHit << "\n";

    sceneObjects.push_back(t);

    UI ui{window, sdlRenderer};
    Scene scene{sceneObjects};
    Camera camera{IMAGE_WIDTH, IMAGE_HEIGHT};
    Renderer renderer{IMAGE_WIDTH, IMAGE_HEIGHT, scene, camera};

    renderer.startRenderAsync(100, 5);
    while (true)
    {
        SDL_Event event;
        SDL_PollEvent(&event);

        if (event.type == SDL_QUIT)
        {
            break;
        }

        ImGui_ImplSDL2_ProcessEvent(&event);

        SDL_RenderClear(sdlRenderer);

        for (int i = 0; i < IMAGE_WIDTH; i++)
        {
            for (int j = 0; j < IMAGE_HEIGHT; j++)
            {
                const auto [r, g, b] = renderer.getOutput()[i][j];
                SDL_SetRenderDrawColor(sdlRenderer, r * 255.0, g * 255.0, b * 255.0, 255);
                SDL_RenderDrawPoint(sdlRenderer, i, j);
            }
        }

        ui.update();

        SDL_RenderPresent(sdlRenderer);
    }

    renderer.stopRender();

    return 0;
}
