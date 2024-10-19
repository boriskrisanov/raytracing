#include <iostream>
#include <sdl.h>

constexpr int windowWidth = 255;
constexpr int windowHeight = 255;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("Ray Tracing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight,
                                          SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    while (true)
    {
        SDL_Event event;
        SDL_PollEvent(&event);


        if (event.type == SDL_QUIT)
        {
            break;
        }

        SDL_RenderClear(renderer);

        for (int i = 0; i < windowWidth; i++)
        {
            for (int j = 0; j < windowHeight; j++)
            {
                SDL_SetRenderDrawColor(renderer, i, j, 255, 255);
                SDL_RenderDrawPoint(renderer, i, j);
            }
        }

        SDL_RenderPresent(renderer);
    }

    return 0;
}
