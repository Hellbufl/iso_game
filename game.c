# include "game.h"

GameState* gstate_init(int fsize[], int screen_size[])
{
    GameState* gstate = (GameState*) calloc(1, sizeof(GameState));

    gstate->field_size[0] = fsize[0];
    gstate->field_size[1] = fsize[1];

    gstate->cell_size[0] = screen_size[0] / fsize[0];
    gstate->cell_size[1] = screen_size[1] / fsize[1];

    gstate_field_init(gstate);

    gstate->ass = ass_init(gstate->field, fsize[0], fsize[1]);
    gstate->path = narr_init(10);

    return gstate;
}

void gstate_destroy(GameState* gstate)
{
    gstate_field_destroy(gstate);
    ass_destroy(gstate->ass);
    narr_destroy(gstate->path);
    free(gstate);
}

void gstate_update(GameState* gstate)
{
    if (gstate->mode < 3) return;

    if (!gstate->ass->found)
    {
        ass_step(gstate->ass);
        return;
    }

    ass_backtrack(gstate->ass, gstate->path);
    gstate->mode = 1;
    
    if (gstate->path == NULL)
    {
        perror("gstate_update(): path == NULL");
        exit(1);
    }
}

void gstate_field_init(GameState* gstate)
{
    gstate->field = (Node***) calloc(gstate->field_size[0], sizeof(Node**));

    for (int ix = 0; ix < gstate->field_size[0]; ix++)
    {
        gstate->field[ix] = (Node**) calloc(gstate->field_size[1], sizeof(Node*));

        for (int iy = 0; iy < gstate->field_size[1]; iy++)
        {
            gstate->field[ix][iy] = node_init();
            gstate->field[ix][iy]->x = ix;
            gstate->field[ix][iy]->y = iy;
        }
    }
}

void gstate_field_destroy(GameState* gstate)
{
    for (int ix = 0; ix < gstate->field_size[0]; ix++)
    {
        for (int iy = 0; iy < gstate->field_size[1]; iy++)
            free(gstate->field[ix][iy]);

        free(gstate->field[ix]);
    }

    free(gstate->field);
}

void gstate_click(GameState* gstate, int click_pos[])
{
    if (gstate->mode > 2) return;
    
    // clearing stuff
    if (gstate->path != NULL)
        if (gstate->path->len > 0) narr_clear(gstate->path);
    if (gstate->ass->open_heap->len > 0) narr_clear(gstate->ass->open_heap);
    if (gstate->ass->closed_narr->len > 0) narr_clear(gstate->ass->closed_narr);

    int cell[2] = { click_pos[0] / gstate->cell_size[0],
                            click_pos[1] / gstate->cell_size[1] };
    
    Node* clicked_node = gstate->field[cell[0]][cell[1]];

    switch (gstate->mode)
    {
        case 0:
            if (gstate->ass->start == clicked_node) break;
            if (gstate->ass->dest == clicked_node) break;
            clicked_node->walkable ^= 1;
            break;
        
        case 1:
            if (!clicked_node->walkable) break;
            if (gstate->ass->dest == clicked_node) break;
            gstate->ass->start = clicked_node;
            break;
        
        case 2:
            if (!clicked_node->walkable) break;
            if (gstate->ass->start == clicked_node) break;
            gstate->ass->dest = clicked_node;
            break;
    }
}

void gstate_keypress(GameState* gstate, SDL_KeyboardEvent event)
{
    if (gstate->path != NULL && gstate->path->len > 0)
        narr_clear(gstate->path);
        
    switch (event.keysym.scancode)
    {
    case SDL_SCANCODE_O:
        gstate->mode = 0;
        break;

    case SDL_SCANCODE_A:
        gstate->mode = 1;
        break;

    case SDL_SCANCODE_B:
        gstate->mode = 2;
        break;
    
    case SDL_SCANCODE_SPACE:
        if (gstate->ass->start == NULL) break;
        if (gstate->ass->dest == NULL) break;

        gstate->mode = 3;

        ass_reset(gstate->ass);

        break;

    default: {}
    }
}