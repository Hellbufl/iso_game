#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct AStar
{
    struct Node*** field;
    int f_width;
    int f_height;

    struct Node* start;
    struct Node* dest;
    struct NodeHeap* open_heap;
    struct NodeArray* closed_narr;

    int stepcount;
    int found;

} AStar;

AStar* ass_init(Node*** init_field, int init_width, int init_height, Node* init_start, Node* init_dest);
void ass_reset(AStar* ass);
int get_neighbours(AStar* ass, Node* node);
void ass_step(AStar* ass);
NodeArray* ass_find_direct_path(AStar* ass, NodeArray* path, int first, int last);
int ass_raycast(AStar* ass, Node* start, Node* end);