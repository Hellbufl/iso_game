#include <stdlib.h>
#include <stdio.h>
#include <math.h>
// #include "minheap.h"
#include "util.h"

#define TARGET_CAPACITY_FACTOR 2
#define MAX_CAPACITY_FACTOR 4

typedef struct Node
{
    int x;
    int y;

    int G;
    int H;
    int F;

    int walkable;
    
    struct Node* parent;

} Node;

typedef struct NodeArray
{
    struct Node** elements;
    int len;
    int cap;

} NodeArray;

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

// -- A* functions -- //
AStar* astar_init(Node*** init_field, int init_width, int init_height);
void astar_reset(AStar* astar);
void astar_destroy(AStar* astar);
// void astar_search(AStar* astar);
NodeArray* get_neighbours(AStar* astar, Node* node, NodeArray* nbs_narr);
void astar_step(AStar* astar);
void astar_backtrack(AStar* astar, NodeArray* path);
NodeArray* astar_find_direct_path(AStar* astar, NodeArray* path, int first, int last);
int astar_raycast(AStar* astar, Node* start, Node* end);

// -- Node functions -- //
Node* node_init();
void node_reset(Node* node);
int node_cal_G(Node* node, Node* parent);
int node_cal_H(Node* node, Node* dest);
void node_cal_F(Node* node, Node* parent, Node* dest);
void node_swap(Node** e1, Node** e2);

// -- NodeArray functions -- //
NodeArray* narr_init(int capacity);
void narr_expand(NodeArray* narr, int add_cap);
void narr_balance(NodeArray* narr);
void narr_destroy(NodeArray* narr);
void narr_obliterate(NodeArray* narr);
void narr_clear(NodeArray* narr);
void narr_set(NodeArray* narr, int i, Node* node);
void narr_add(NodeArray* narr, Node* node);
void narr_remove(NodeArray* narr, Node* node);
int narr_contains(NodeArray* narr, Node* node);
void narr_append(NodeArray* narr_1, int i, NodeArray* narr_2, int k);
NodeArray* narr_join(NodeArray* narr_1, int i, NodeArray* narr_2, int k);
void narr_reverse(NodeArray* narr);
void narr_print(NodeArray* narr);

// -- MinHeap functions -- //
void heap_sortup(NodeArray* heap, int i);
void heap_sortdown(NodeArray* heap);
void heap_add(NodeArray* heap, Node* node);
Node* heap_take(NodeArray* heap);
int heap_height(NodeArray* heap);
int less_expensive(NodeArray* heap, int i, int k);
void heap_show(NodeArray* heap);