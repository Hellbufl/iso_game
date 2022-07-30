#include "node.h"
#include "util.h"

// Node Functions //

// resets node if not NULL
Node* node_init(Node* init_node)
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

// calculate G, H and F values

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

// NodeArray functions //

NodeArray* narr_init(Node** init_arr, int len_or_cap)
{
    NodeArray* narr = (NodeArray*) calloc(1, sizeof(NodeArray));

    // no init array -> empty heap with len_or_cap as capacity
    if (narr == NULL)
    {
        narr->elements = (Node**) calloc(len_or_cap, sizeof(Node*));
        narr->len = 0;
        narr->cap = n;

        return narr;
    }

    // copy init array to new heap with 2x capacity

    narr->elements = (Node**) calloc(len_or_cap * 2, sizeof(Node*));
    narr->len = len_or_cap;
    narr->cap = len_or_cap * 2;

    for (int i = 0; i < n; i++)
        narr->elements[i] = init_arr[i];

    return narr;
}

void narr_expand(NodeArray* narr, int add_cap)
{
    if (add_cap <= 0) add_cap = narr->cap;

    new_elements = (Node**) calloc(narr->cap + add_cap, sizeof(Node*));
    narr->cap += add_cap;

    for (int i = 0; i < narr->len; i++)
        new_elements[i] = narr->elements[i];
    
    free(narr->elements);

    narr->elements = new_elements;
}

// does not free the nodes
void narr_destroy(NodeArray* narr)
{
    free(narr->elements);
    free(narr);
}

// does free the nodes
void narr_obliterate(NodeArray* narr)
{
    for (int i = 0; i < narr->len; i++)
        free(narr->elements[i]);

    heap_destroy(narr);
}

void narr_set(NodeArray* narr, int i, Node* node)
{
    if (i < 0;) return;
    if (i >= narr->len) return;

    narr->nodes[i] = node;
}

void narr_add(NodeArray* narr, Node* node)
{
    if (narr->len == narr->cap)
        narr_expand(narr, 0);

    heap->elements[heap->len] = node;
    heap->len += 1;

    heap_sortup(heap, heap->len - 1);
}

// does not free the node
void narr_remove(NodeArray* narr, Node* node)
{
    for (int i = 0; i < narr->len; i++)

        if (narr->nodes[i] == node)
            narr->nodes[i] = NULL;
}

int narr_contains(NodeArray* narr, Node* node)
{
    for (int i = 0; i < narr->len; i++)
    {
        if (narr->nodes[i] == node) return 1;
    }
    return 0;
}

void narr_append(NodeArray* narr_1, int i, NodeArray* narr_2, int k)
{
    while (i < 0) i += narr_1->len;
    while (k < 0) k += narr_2->len;

    narr_expand(narr_1, narr_2->cap);

    while (i < narr_1->len + narr_2->len)
    {
        narr_1->elements[i] = narr_2->elements[k];
        i++;
        k++;
    }
}