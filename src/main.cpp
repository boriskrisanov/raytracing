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

#include "random.hpp"

using Color = Vector3;

// Window
const double ASPECT_RATIO = 16.0 / 9.0;
const int IMAGE_WIDTH = 800;
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

const Vector3 VIEWPORT_UPPER_LEFT = CAMERA_CENTER - Vector3{0, 0, FOCAL_LENGTH} - VIEWPORT_POSITIVE_X / 2 -
    VIEWPORT_POSITIVE_Y / 2;
const Vector3 ORIGIN_PIXEL_LOCATION = VIEWPORT_UPPER_LEFT + 0.5 * (PIXEL_DELTA_X + PIXEL_DELTA_Y);

std::vector<SceneObject*> sceneObjects;

void drawUI()
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // ImGui::ShowDemoWindow();
}

const int numBounces = 10;
const int numSamples = 10;

HitInfo castRay(Ray ray)
{
    HitInfo h{};
    for (SceneObject* sceneObject : sceneObjects)
    {
        HitInfo h2 = sceneObject->intersects(ray, {0.0001, 100000000});
        if (h2.didHit)
        {
            h2.material = sceneObject->material;
            if (!h.didHit || h2.rayParameter < h.rayParameter)
            {
                h = h2;
            }
        }
    }
    return h;
}

Color rayColor(Ray ray)
{
    Color rayColor = {1, 1, 1};
    Color incomingLight = {0, 0, 0};

    for (int i = 0; i < numBounces; i++)
    {
        HitInfo h = castRay(ray);

        if (!h.didHit)
        {
            auto a = 0.5*(ray.direction.normalised().y + 1.0);
            const Vector3 skyColor = (1.0-a)*Color(1.0, 1.0, 1.0) + a*Color(0.5, 0.7, 1.0);
            return rayColor * skyColor;
        }

        incomingLight += rayColor * (h.material.emissionColor * h.material.emissionStrength);
        rayColor *= 0.5 * h.material.color;

        const Vector3 unitVector = h.normal + randomUnitVector();
        ray = Ray{h.point, unitVector};
    }

    return incomingLight;
}

Color pixelColor(Ray ray)
{
    Color color = {0, 0, 0};
    for (int i = 0; i < numSamples; i++)
    {
        color += rayColor(ray);
    }
    return color / numSamples;
}

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("Ray Tracing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, IMAGE_WIDTH,
                                          IMAGE_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    if (window == nullptr)
    {
        std::cout << "Failed to initialise SDL window: " << SDL_GetError() << "\n";
        return 1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    const ImGuiIO& io = ImGui::GetIO();

    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    const Material m1{Vector3{1, 1, 1}};
    const Material m2{Vector3{1, 1, 1}};
    const Material lightMaterial{Vector3{1, 1, 1}, {1, 1, 1}, 1};

    sceneObjects.push_back(new Sphere{{0, 0, -1}, 0.5, m1});
    sceneObjects.push_back(new Sphere{{0, -100.5, 0}, 100, m2});
    // sceneObjects.push_back(new Sphere{{0, 1, -1}, 0.25, lightMaterial});

    // std::array<std::array<Color, windowHeight>, windowWidth>> a;

    while (true)
    {
        SDL_Event event;
        SDL_PollEvent(&event);

        if (event.type == SDL_QUIT)
        {
            break;
        }

        ImGui_ImplSDL2_ProcessEvent(&event);

        SDL_RenderClear(renderer);

        for (int i = 0; i < IMAGE_WIDTH; i++)
        {
            for (int j = 0; j < IMAGE_HEIGHT; j++)
            {
                auto pixel_center = ORIGIN_PIXEL_LOCATION + (i * PIXEL_DELTA_X) + (j * PIXEL_DELTA_Y);
                auto ray_direction = pixel_center - CAMERA_CENTER;
                Ray r(CAMERA_CENTER, ray_direction);
                Color c = pixelColor(r);

                SDL_SetRenderDrawColor(renderer, c.x * 255.0, c.y * 255.0, c.z * 255.0, 255);
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
