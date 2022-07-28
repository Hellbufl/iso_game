#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct Heap
{
    struct Node** elements;
    int len;
    int capacity;
} Heap;

Heap* heap_init(Node** init_array, int caplen);
void heap_destroy(Heap* heap);
void heap_obliterate(Heap* heap);
void heap_sortup(Heap* heap, int i);
void heap_sortdown(Heap* heap);
void heap_add(Heap* heap, Node* node);
Node* heap_take(Heap* heap);
int heap_height(Heap* heap);
void heap_show(Heap* heap);