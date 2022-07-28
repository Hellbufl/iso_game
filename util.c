#include "util.h"
#include "node.h"
#include "minheap.h"

// utility functions //

int ipow(int b, int e)
{
    int result = 1;

    for (int i = 0; i < e; i++)
        result *= b;

    return result;

}

void swap(Node* e1, Node* e2)
{
    Node tmp = *e1;
    *e1 = *e2;
    *e2 = tmp;
}

int less_expensive(Heap* heap, int i, int k)
{
    if (i >= heap->len) return 0;
    if (k >= heap->len) return 1;
    if (heap->elements[i]->F > heap->elements[k]->F) return 0;
    if (heap->elements[i]->F < heap->elements[k]->F) return 1;
    if (heap->elements[i]->H > heap->elements[k]->H) return 1;
    return 0;
}

int node_in_arr(Node* node, Node** arr, int arr_len)
{
    for (int i = 0; i < arr_len; i++)
    {
        if (arr[i] == node) return 1;
    }
    return 0;
}