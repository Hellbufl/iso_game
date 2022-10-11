#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "graphics.h"

#define TICKRATE 30

int main(int argc, char *argv[])
{
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    graph_init(&window, &renderer);

    int fsize[] = { 16, 9 };
    int screen_size[] = { SCREEN_WIDTH, SCREEN_HEIGHT };

    GameState* gstinky = gstate_init(fsize, screen_size);

    struct timespec last_tspec, now_tspec;
    long delta_time, since_tick = 0; //nanoseconds
    clock_gettime(CLOCK_REALTIME, &last_tspec);

    // int fcount = 0;
    // int fadd = 0;
    // int famount = 100;

    const int ticklen = 1e9 / TICKRATE;

    // main loop
    while (gstinky->is_running)
    {
        clock_gettime(CLOCK_REALTIME, &now_tspec);
        delta_time = (now_tspec.tv_sec - last_tspec.tv_sec) * 1e9 + now_tspec.tv_nsec - last_tspec.tv_nsec;

        last_tspec.tv_sec = now_tspec.tv_sec;
        last_tspec.tv_nsec = now_tspec.tv_nsec;

        since_tick += delta_time;

        while(since_tick > ticklen)
        {
            gstate_tick(gstinky);
            since_tick -= ticklen;
        }

        graph_draw(renderer, gstinky);

        SDL_RenderPresent(renderer);

        // fcount++;
        // fadd += delta_time;
        // if (fcount == famount)
        // {
        //     printf("%d\n",(int) (famount * 1e9 / fadd));
        //     fadd = 0;
        //     fcount = 0;
        // }
    }

    gstate_destroy(gstinky);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();

    return EXIT_SUCCESS;
}