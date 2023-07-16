#include "graphics.h"

void graph_init(SDL_Window** window, SDL_Renderer** renderer)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "Could not initialize SDL2: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    *window = SDL_CreateWindow(  "Drag-On Deez Nuts",
                                100, 100,
                                SCREEN_WIDTH, SCREEN_HEIGHT,
                                SDL_WINDOW_SHOWN                );

    if (window == NULL)
    {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    *renderer = SDL_CreateRenderer(  *window, -1,
                                    SDL_RENDERER_ACCELERATED |
                                    SDL_RENDERER_PRESENTVSYNC   );
    if (renderer == NULL)
    {
        SDL_DestroyWindow(*window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

void graph_draw(SDL_Renderer* renderer, GameState* gstate)
{
    SDL_Rect node_rect;

    SDL_SetRenderDrawColor(renderer, 42, 42, 42, 255);
    SDL_RenderClear(renderer);

    for (int ix = 0; ix < gstate->field_size[0]; ix++)
    {
        for (int iy = 0; iy < gstate->field_size[1]; iy++)
        {
            Node* current_node = gstate->field[ix][iy];

            node_rect.x = ix * gstate->cell_size[0];
            node_rect.y = iy * gstate->cell_size[1];
            node_rect.w = gstate->cell_size[0] + 1;
            node_rect.h = gstate->cell_size[1] + 1;

            // draw nodes in open heap and closed list

            if (narr_contains(gstate->astar->open_heap, current_node))
            {
                SDL_SetRenderDrawColor(renderer, 120, 120, 169, 255);
                SDL_RenderFillRect(renderer, &node_rect);
            }

            if (narr_contains(gstate->astar->closed_narr, current_node))
            {
                SDL_SetRenderDrawColor(renderer, 120, 169, 120, 255);
                SDL_RenderFillRect(renderer, &node_rect);
            }

            // draw start and destination

            if (current_node == gstate->astar->start)
            {
                SDL_SetRenderDrawColor(renderer, 120, 69, 255, 255);
                SDL_RenderFillRect(renderer, &node_rect);
            }

            else if (current_node == gstate->astar->dest)
            {
                SDL_SetRenderDrawColor(renderer, 162, 255, 43, 255);
                SDL_RenderFillRect(renderer, &node_rect);
            }

            // draw path

            if (narr_contains(gstate->path, current_node))
            {
                SDL_SetRenderDrawColor(renderer, 232, 169, 69, 255);
                SDL_RenderFillRect(renderer, &node_rect);
            }

            if (narr_contains(gstate->direct_path, current_node))
            {
                SDL_SetRenderDrawColor(renderer, 232, 95, 42, 255);
                SDL_RenderFillRect(renderer, &node_rect);
            }

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &node_rect);

            if (current_node->walkable) continue;

            // draw obstacles

            node_rect.w -= 1;
            node_rect.h -= 1;

            SDL_RenderDrawLine(renderer, node_rect.x, node_rect.y, node_rect.x + node_rect.w, node_rect.y + node_rect.h);
            SDL_RenderDrawLine(renderer, node_rect.x + node_rect.w, node_rect.y, node_rect.x, node_rect.y + node_rect.h);

            node_rect.x += node_rect.w / 4;
            node_rect.y += node_rect.h / 4;
            node_rect.w = node_rect.w / 2 + 1;
            node_rect.h = node_rect.h / 2 + 1;

            SDL_SetRenderDrawColor(renderer, 42, 42, 42, 255);
            SDL_RenderFillRect(renderer, &node_rect);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &node_rect);
        }
    }
}