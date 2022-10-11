#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "minheap.h"
#include "util.h"

typedef struct AStar
{
    Node*** field;
    int f_width;
    int f_height;

    Node* start;
    Node* dest;
    NodeArray* open_heap;
    NodeArray* closed_narr;

    int stepcount;
    int found;

} AStar;

AStar* astar_init(Node*** init_field, int init_width, int init_height);
void astar_reset(AStar* astar);
void astar_destroy(AStar* astar);
NodeArray* get_neighbours(AStar* astar, Node* node, NodeArray* nbs_narr);
void astar_step(AStar* astar);
void astar_backtrack(AStar* astar, NodeArray* path);
NodeArray* astar_find_direct_path(AStar* astar, NodeArray* path, int first, int last);
int astar_raycast(AStar* astar, Node* start, Node* end);