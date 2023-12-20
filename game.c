# include "game.h"

GameState* gstate_init(int fsize[], int screeSize[])
{
    GameState* gstate = (GameState*) calloc(1, sizeof(GameState));

    gstate->fieldSize[0] = fsize[0];
    gstate->fieldSize[1] = fsize[1];

    gstate->cellSize[0] = screeSize[0] / fsize[0];
    gstate->cellSize[1] = screeSize[1] / fsize[1];

    gstate_field_init(gstate);

    gstate->aStar = AStar_Init(gstate->field, fsize[0], fsize[1]);
    gstate->path = NArr_Init(10);
    gstate->directPath = NArr_Init(100);
    gstate->isRunning = 1;

    return gstate;
}

void gstate_destroy(GameState* gstate)
{
    gstate_field_destroy(gstate);
    AStar_Destroy(gstate->aStar);
    NArr_Destroy(gstate->path);
    NArr_Destroy(gstate->directPath);
    free(gstate);
}

void gstate_tick(GameState* gstate)
{
    gstate_input(gstate);
    gstate_update(gstate);
}

void gstate_update(GameState* gstate)
{
    if (gstate->mode < 3) return;

    if (!gstate->aStar->found)
    {
        AStar_Step(gstate->aStar);
        return;
    }

    AStar_Backtrack(gstate->aStar, gstate->path);
    gstate->mode = 1;

    if (gstate->path == NULL)
    {
        perror("gstate_update(): path == NULL");
        exit(1);
    }

    NArr_Clear(gstate->directPath);
    AStar_FindDirectPath(gstate->aStar, gstate->path, gstate->directPath);
}

void gstate_input(GameState* gstate)
{
    int clickPos[2];

    // check events
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            gstate->isRunning = 0;
            break;
        
        case SDL_MOUSEBUTTONDOWN:
            SDL_GetMouseState(&clickPos[0], &clickPos[1]);
            gstate_click(gstate, clickPos);
            break;
        
        case SDL_KEYDOWN:
            gstate_keypress(gstate, event.key);
            break;

        default: {}
        }
    }
}

void gstate_field_init(GameState* gstate)
{
    gstate->field = (Node***) calloc(gstate->fieldSize[0], sizeof(Node**));

    for (int ix = 0; ix < gstate->fieldSize[0]; ix++)
    {
        gstate->field[ix] = (Node**) calloc(gstate->fieldSize[1], sizeof(Node*));

        for (int iy = 0; iy < gstate->fieldSize[1]; iy++)
        {
            gstate->field[ix][iy] = Node_Init();
            gstate->field[ix][iy]->x = ix;
            gstate->field[ix][iy]->y = iy;
        }
    }
}

void gstate_field_destroy(GameState* gstate)
{
    for (int ix = 0; ix < gstate->fieldSize[0]; ix++)
    {
        for (int iy = 0; iy < gstate->fieldSize[1]; iy++)
            free(gstate->field[ix][iy]);

        free(gstate->field[ix]);
    }

    free(gstate->field);
}

void gstate_click(GameState* gstate, int clickPos[])
{
    if (gstate->mode > 2) return;
    
    // clearing stuff
    if (gstate->path != NULL)
        if (gstate->path->len > 0) NArr_Clear(gstate->path);
    if (gstate->directPath != NULL)
        if (gstate->path->len > 0) NArr_Clear(gstate->directPath);

    if (gstate->aStar->openHeap->len > 0) NArr_Clear(gstate->aStar->openHeap);
    if (gstate->aStar->closedNArr->len > 0) NArr_Clear(gstate->aStar->closedNArr);
    // NArr_Clear(gstate->path);
    // NArr_Clear(gstate->directPath);
    // NArr_Clear(gstate->aStar->openHeap);
    // NArr_Clear(gstate->aStar->closedNArr);

    int cell[2] = { clickPos[0] / gstate->cellSize[0],
                            clickPos[1] / gstate->cellSize[1] };
    
    Node* clicked_node = gstate->field[cell[0]][cell[1]];

    switch (gstate->mode)
    {
        case 0:
            if (gstate->aStar->start == clicked_node) break;
            if (gstate->aStar->dest == clicked_node) break;
            clicked_node->walkable ^= 1;
            break;
        
        case 1:
            if (!clicked_node->walkable) break;
            if (gstate->aStar->dest == clicked_node) break;
            gstate->aStar->start = clicked_node;
            break;
        
        case 2:
            if (!clicked_node->walkable) break;
            if (gstate->aStar->start == clicked_node) break;
            gstate->aStar->dest = clicked_node;
            break;
    }
}

void gstate_keypress(GameState* gstate, SDL_KeyboardEvent event)
{
    if (gstate->path != NULL && gstate->path->len > 0)
    {
        NArr_Clear(gstate->path);
        NArr_Clear(gstate->directPath);
    }
        
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
        if (gstate->aStar->start == NULL) break;
        if (gstate->aStar->dest == NULL) break;

        gstate->mode = 3;

        AStar_Reset(gstate->aStar);

        break;

    default: {}
    }
}