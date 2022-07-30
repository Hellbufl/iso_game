#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// typedef struct NodeHeap
// {
//     struct Node** elements;
//     int len;
//     int cap;
    
// } NodeHeap;

// NodeHeap* heap_init(Node** init_arr, int n)
// void heap_destroy(NodeHeap* heap);
// void heap_obliterate(NodeHeap* heap);

// void heap_sortup(NodeHeap* heap, int i);
// void heap_sortdown(NodeHeap* heap);
// void heap_add(NodeHeap* heap, Node* node);
// Node* heap_take(NodeHeap* heap);
// int heap_height(NodeHeap* heap);
// void heap_show(NodeHeap* heap);

void heap_sortup(NodeArray* heap, int i);
void heap_sortdown(NodeArray* heap);
void heap_add(NodeArray* heap, Node* node);
Node* heap_take(NodeArray* heap);
int heap_height(NodeArray* heap);
void heap_show(NodeArray* heap);