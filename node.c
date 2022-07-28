#include "node.h"
#include "util.h"

// resets node if not NULL
Node* node_init(Node* node)
{
    Node* new_node = (Node*) calloc(1, sizeof(Node));

    if (node == NULL) 
    {
        new_node->parent = new_node;
        return new_node;
    }

    *node = *new_node;
    free(new_node);
    node->parent = node;

    return node;
}

// calculate G, H and F values //

int node_cal_G(Node* node, Node* parent)
{
    if (node->x == parent->x || node->y == parent->y)
        return parent->G + 10;
    
    return parent->G + 14;
}

int node_cal_H(Node* node, Node* dest)
{
    int delta_x = abs(dest->x - node->x);
    int delta_y = abs(dest->y - node->y);

    if (delta_x < delta_y)
        return 14 * delta_x + 10 * (delta_y - delta_x);
    
    if (delta_x > delta_y)
        return 14 * delta_y + 10 * (delta_x - delta_y);
    
    return 14 * delta_x;
}

void node_cal_F(Node* node, Node* parent, Node* dest)
{
    int new_G = node_cal_G(parent);

    if (new_G < node->G || node->G == 0)
    {
        node->G = new_G;
        node->parent = parent;
    }

    node->H = node_cal_H(dest);
    node->F = node->G + node->H;
}