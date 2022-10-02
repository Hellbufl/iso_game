#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "util.h"
#include "node.h"

void heap_sortup(NodeArray* heap, int i);
void heap_sortdown(NodeArray* heap);
void heap_add(NodeArray* heap, Node* node);
Node* heap_take(NodeArray* heap);
int heap_height(NodeArray* heap);
int less_expensive(NodeArray* heap, int i, int k);
void heap_show(NodeArray* heap);