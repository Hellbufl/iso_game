#include "node.h"

// Node Functions //

Node* node_init()
{
    Node* new_node = (Node*) calloc(1, sizeof(Node));
    new_node->walkable = 1;
    new_node->parent = new_node;
    return new_node;
}

void node_reset(Node* node)
{
    node->G = 0;
    node->H = 0;
    node->F = 0;
    node->parent = node;
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

    if (delta_x == delta_y)
        return 14 * delta_x;
    
    return 14 * MIN(delta_x, delta_y) + 10 * abs(delta_y - delta_x);
}

void node_cal_F(Node* node, Node* parent, Node* dest)
{
    int new_G = node_cal_G(node, parent);

    if (new_G < node->G || node->G == 0)
    {
        node->G = new_G;
        node->parent = parent;
    }

    node->H = node_cal_H(node, dest);
    node->F = node->G + node->H;
}

void node_swap(Node** e1, Node** e2)
{
    Node* tmp = *e1;
    *e1 = *e2;
    *e2 = tmp;
}

// NodeArray functions //

NodeArray* narr_init(int capacity)
{
    NodeArray* narr = (NodeArray*) calloc(1, sizeof(NodeArray));

    if (narr == NULL)
    {
        perror("Failed to allocate memory for NodeArray");
        exit(1);
    }

    narr->elements = (Node**) calloc(capacity, sizeof(Node*));
    narr->len = 0;
    narr->cap = capacity;

    return narr;
}

void narr_expand(NodeArray* narr, int add_cap)
{
    if (narr == NULL) return;

    if (add_cap <= 0)
        add_cap = narr->cap;

    Node** new_elements = (Node**) calloc(narr->cap + add_cap, sizeof(Node*));
    narr->cap += add_cap;

    for (int i = 0; i < narr->len; i++)
        new_elements[i] = narr->elements[i];
    
    free(narr->elements);

    narr->elements = new_elements;
}

// does NOT free the nodes
void narr_destroy(NodeArray* narr)
{
    if (narr == NULL) return;

    free(narr->elements);
    free(narr);
}

// does free the nodes
void narr_obliterate(NodeArray* narr)
{
    if (narr == NULL) return;

    for (int i = 0; i < narr->len; i++)
        free(narr->elements[i]);

    narr_destroy(narr);
}

// does NOT free the nodes
void narr_clear(NodeArray* narr)
{
    if (narr == NULL) return;

    for (int i = 0; i < narr->len; i++)
        narr->elements[i] = NULL;
    
    narr->len = 0;
}

void narr_set(NodeArray* narr, int i, Node* node)
{
    if (i < 0) return;
    if (i >= narr->len) return;

    narr->elements[i] = node;
}

void narr_add(NodeArray* narr, Node* node)
{
    if (narr->len == narr->cap)
        narr_expand(narr, 0);

    narr->elements[narr->len] = node;
    narr->len += 1;
}

// does NOT free the node
void narr_remove(NodeArray* narr, Node* node)
{
    for (int i = 0; i < narr->len; i++)

        if (narr->elements[i] == node)
            narr->elements[i] = NULL;
}

int narr_contains(NodeArray* narr, Node* node)
{
    if (narr == NULL) return 0;

    for (int i = 0; i < narr->len; i++)
    {
        if (narr->elements[i] == node) return 1;
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

NodeArray* narr_join(NodeArray* narr_1, int i, NodeArray* narr_2, int k)
{
    NodeArray* new_narr = narr_init(narr_1->cap + narr_2->cap);

    while (i < 0) i += narr_1->len;
    while (k < 0) k += narr_2->len;

    int new_len_1 = i % narr_1->len;
    int new_len_2 = narr_2->len - k % narr_2->len;

    for (i = 0; i < new_len_1; i++)
        new_narr->elements[i] = narr_1->elements[i];

    for (i = 0; i < new_len_2; i++)
        new_narr->elements[new_len_1 + i] = narr_2->elements[k + i];
    
    return new_narr;
}