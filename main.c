#include <stdlib.h>
#include <stdio.h>
#include "graphics.h"

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "Could not initialize SDL2: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *window = SDL_CreateWindow("Drag-On Deez Nuts",
                                          100, 100,
                                          SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);

    if (window == NULL)
    {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
                                                SDL_RENDERER_ACCELERATED |
                                                SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
    {
        SDL_DestroyWindow(window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    int fsize[] = { 16, 9 };
    int screen_size[] = { SCREEN_WIDTH, SCREEN_HEIGHT };

    GameState* gstinky = gstate_init(fsize, screen_size);
    int click_pos[2];

    int state = 0;

    // main loop
    while (state == 0)
    {
        // check events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                state = 1;
                break;
            
            case SDL_MOUSEBUTTONDOWN:
                SDL_GetMouseState(&click_pos[0], &click_pos[1]);
                gstate_click(gstinky, click_pos);
                break;
            
            case SDL_KEYDOWN:
                gstate_keypress(gstinky, event.key);
                break;

            default: {}
            }
        }

        gstate_update(gstinky);
        draw_field(renderer, gstinky);

        SDL_RenderPresent(renderer);

        // SDL_Delay(200.0f);
    }

    gstate_destroy(gstinky);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();

    return EXIT_SUCCESS;
}