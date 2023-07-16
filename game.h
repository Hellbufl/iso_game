#include <SDL2/SDL.h>
#include "vecmath.h"
#include "astar.h"

typedef struct GameState {
    int is_running;
    int mode; // 0-place/remove obstacles | 1-place A | 2-place B | 3-search one step per tick

    int field_size[2];
    int cell_size[2];

    Node*** field;
    AStar* astar;

    NodeArray* path;
    NodeArray* direct_path;

} GameState;

GameState* gstate_init(int fsize[], int screen_size[]);
void gstate_destroy(GameState* gstate);
void gstate_tick(GameState* gstate);
void gstate_update(GameState* gstate);
void gstate_input(GameState* gstate);
void gstate_field_init(GameState* gstate);
void gstate_field_destroy(GameState* gstate);
void gstate_click(GameState* gstate, int* click_pos);
void gstate_keypress(GameState* gstate, SDL_KeyboardEvent event);