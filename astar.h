#include <stdlib.h>
#include <stdio.h>

typedef struct AStar
{
    struct Node*** field;
    int f_width;
    int f_height;

    struct Node* start;
    struct Node* dest;
    struct Heap* heap_open;
    struct Node** closed;
    int closed_len;
    int found;
    int stepcount;

} AStar;

AStar* ass_init(Node*** init_field, int init_width, int init_height, Node* init_start, Node* init_dest)
void ass_reset(AStar* ass);
int get_neighbours(AStar* ass, Node* node, Node* nbs_arr[]);