#include "game.h"

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

void graph_init(SDL_Window** window, SDL_Renderer** renderer);
void graph_draw(SDL_Renderer *renderer, GameState* gstate);