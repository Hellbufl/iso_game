#include <stdlib.h>
#include <stdio.h>
#include "util.h"

typedef struct node
{
    int x;
    int y;

    int G;
    int H;
    int F;

    int walkable;
    
    struct node* parent;

} Node;

typedef struct nodeArray
{
    struct node** elements;
    int len;
    int cap;

} NodeArray;

Node* node_init(Node* init_node);
int node_cal_G(Node* node, Node* parent);
int node_cal_H(Node* node, Node* dest);
void node_cal_F(Node* node, Node* parent, Node* dest);
void node_swap(Node* e1, Node* e2);

NodeArray* narr_init(Node** init_arr, int len_or_cap);
void narr_expand(NodeArray* narr);
void narr_destroy(NodeArray* narr);
void narr_obliterate(NodeArray* narr);
void narr_set(NodeArray* narr, int i, Node* node);
void narr_add(NodeArray* narr, Node* node);
void narr_remove(NodeArray* narr, Node* node);
int narr_contains(NodeArray* narr, Node* node);
void narr_append(NodeArray* narr_1, int i, NodeArray* narr_2, int k);