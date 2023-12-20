#include <SDL2/SDL.h>
#include "vecmath.h"
#include "astar.h"

typedef struct GameState {
    int isRunning;
    int mode; // 0-place/remove obstacles | 1-place A | 2-place B | 3-search one step per tick

    int fieldSize[2];
    int cellSize[2];

    Node*** field;
    AStar* aStar;

    NodeArray* path;
    NodeArray* directPath;

} GameState;

GameState* gstate_init(int fsize[], int screeSize[]);
void gstate_destroy(GameState* gstate);
void gstate_tick(GameState* gstate);
void gstate_update(GameState* gstate);
void gstate_input(GameState* gstate);
void gstate_field_init(GameState* gstate);
void gstate_field_destroy(GameState* gstate);
void gstate_click(GameState* gstate, int* clickPos);
void gstate_keypress(GameState* gstate, SDL_KeyboardEvent event);