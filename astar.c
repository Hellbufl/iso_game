#include "astar.h"
#include "node.h"
#include "minheap.h"
#include "util.h"

// not for resetting
AStar* ass_init(Node*** init_field, int init_width, int init_height, Node* init_start, Node* init_dest)
{
    AStar* ass = (AStar*) calloc(1, sizeof(AStar));

    ass->field = init_field;
    ass->f_width = init_width;
    ass->f_height = init_height;
    ass->start = init_start;
    ass->dest = init_dest;

    ass->open_heap = narr_init(NULL, 10);
    heap_add(ass->open_heap, ass->start);

    ass->closed_narr = narr_init(NULL, 10);

    return ass;
}

void ass_reset(AStar* ass)
{
    new_ass = ass_init(ass->field, ass->f_width, ass->f_height, ass->start, ass->dest);
    *ass = *new_ass;
}

// returns number of neighbours found 
int get_neighbours(AStar* ass, Node* node)
{
    NodeArray* nbs_narr = narr_init(NULL, 9);

    for (int i = 0; i < 9; i++)
    {
        int x = node->x + (i % 3) - 1;
        int y = node->y + i / 3 - 1;

        if (x == y == 0) continue;

        if (x < 0) continue;
        if (y < 0) continue;

        if (x >= ass->f_width) continue;
        if (y >= ass->f_height) continue;

        Node* current_node = ass->field[x][y];

        if (!current_node->walkable) continue;

        if (narr_contains(ass->closed_narr, current_node)) continue;

        // check the two direct adjacent nodes so you dont cut corners
        if (x != node->x && y != node->y)
        {
            Node* adj_1 = ass->field[node->x][y];
            Node* adj_2 = ass->field[x][node->y];

            if (!(adj_1->walkable && adj_2->walkable)) continue;
        }

        narr_add(nbs_narr, current_node);
    }

    return nbs_narr;
}

void ass_step(AStar* ass)
{
    ass->stepcount++;

    Node* node = heap_take(ass->open_heap);

    narr_add(ass->closed_narr, node);

    if (node = ass->dest)
    {
        ass->found = 1;
        return;
    }

    nbs_narr = get_neighbours(ass, node);

    for (int i = 0; i < nbs_narr->len; i++)
    {
        current_nb = nbs_narr->elements[i];
        old_F = current_nb->F;

        node_cal_F(current_nb, node, ass->dest);

        if (current_nb->F != old_F)
            heap_sortdown(ass->open_heap);
        
        if (!narr_contains(ass->open_heap, current_nb))
            heap_add(ass->open_heap, current_nb);
    }

    // TODO find direct path stuff
}