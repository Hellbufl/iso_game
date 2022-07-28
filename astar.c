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

    ass->open = heap_init(NULL, 10);
    heap_add(ass->open, ass->start);

    return ass;
}

void ass_reset(AStar* ass)
{
    new_ass = ass_init(ass->field, ass->f_width, ass->f_height, ass->start, ass->dest);
    *ass = *new_ass;
}

// needs array of min length 9; index 4 will always be empty
// returns number of neighbours found
int get_neighbours(AStar* ass, Node* node, Node* nbs_arr[])
{
    int n = 0;

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

        if (node_in_arr(current_node, ass->closed, ass->closed_len)) continue;

        // check the two direct adjacent nodes so you dont cut corners
        if (x != node->x && y != node->y)
        {
            Node* adj_1 = ass->field[node->x][y];
            Node* adj_2 = ass->field[x][node->y];

            if (!(adj_1->walkable && adj_2->walkable)) continue;
        }

        nbs_arr[i] = current_node;
        n++;
    }

    return n;
}

//TODO: actual pathfinding shit