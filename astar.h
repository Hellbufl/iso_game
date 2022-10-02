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

AStar* ass_init(Node*** init_field, int init_width, int init_height);
void ass_reset(AStar* ass);
void ass_destroy(AStar* ass);
NodeArray* get_neighbours(AStar* ass, Node* node, NodeArray* nbs_narr);
void ass_step(AStar* ass);
void ass_backtrack(AStar* ass, NodeArray* path);
NodeArray* ass_find_direct_path(AStar* ass, NodeArray* path, int first, int last);
int ass_raycast(AStar* ass, Node* start, Node* end);