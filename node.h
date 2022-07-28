#include <stdlib.h>
#include <stdio.h>

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

Node* node_init(Node* node);
int node_cal_G(Node* node, Node* parent);
int node_cal_H(Node* node, Node* destination);
void node_cal_F(Node* node, Node* parent, Node* destination);