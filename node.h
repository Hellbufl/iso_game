#include <stdlib.h>
#include <stdio.h>
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

Node* node_init();
void node_reset(Node* node);
int node_cal_G(Node* node, Node* parent);
int node_cal_H(Node* node, Node* dest);
void node_cal_F(Node* node, Node* parent, Node* dest);
void node_swap(Node** e1, Node** e2);

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