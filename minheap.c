#include "minheap.h"

// heap functions //

// void heap_sortup(NodeHeap* heap, int i)
void heap_sortup(NodeArray* heap, int i)
{
    if (i <= 0) return;

    // parent index
    int ip = (i - 1) / 2;

    if (!less_expensive(heap, i, ip)) return;

    node_swap(&heap->elements[i], &heap->elements[ip]);
    heap_sortup(heap, ip);
}

// void heap_sortdown(NodeHeap* heap)
void heap_sortdown(NodeArray* heap)
{
    int height = heap_height(heap);
    int n = heap->len - int_pow(2, height - 1) + 1;

    for (int e = height - 1; e > 0; e--)
    {
        int start = int_pow(2, e) - 1;
        
        for (int k = start; k < start + n; k++)
        {
            if (k % 2 == 0) continue;

            int ip = (k - 1) / 2;
            int ir = 2 * ip + 2;

            if (less_expensive(heap, k, ir))
            {
                if (less_expensive(heap, k, ip))
                    node_swap(&heap->elements[k], &heap->elements[ip]);
                
                continue;
            }

            if (less_expensive(heap, ir, ip))
                node_swap(&heap->elements[ir], &heap->elements[ip]);
        }

        n = int_pow(2, e - 1);
    }
}

// void heap_add(NodeHeap* heap, Node* node)
void heap_add(NodeArray* heap, Node* node)
{
    if (heap->len == heap->cap)
        // narr_balance(heap);
        narr_expand(heap, -1);

    heap->elements[heap->len] = node;
    heap->len += 1;

    heap_sortup(heap, heap->len - 1);
}

// Node* heap_take(NodeHeap* heap)
Node* heap_take(NodeArray* heap)
{
    if (heap->len == 0) return NULL;

    Node* first = heap->elements[0];

    for (int i = 0; i < heap->len - 1; i++)
        heap->elements[i] = heap->elements[i+1];
    
    heap->elements[heap->len - 1] = NULL;
    heap->len -= 1;

    // narr_balance(heap);

    return first;
}

// int heap_height(NodeHeap* heap)
int heap_height(NodeArray* heap)
{
    if (heap->len == 0) return 0;

    // log2(x) = logy(x) / logy(2)
    return (int) (log((double) heap->len) / log(2.0)) + 1;
}

int less_expensive(NodeArray* heap, int i, int k)
{
    if (i >= heap->len) return 0;
    if (k >= heap->len) return 1;
    if (heap->elements[i]->F > heap->elements[k]->F) return 0;
    if (heap->elements[i]->F < heap->elements[k]->F) return 1;
    if (heap->elements[i]->H > heap->elements[k]->H) return 0;
    return 1;
}

// void heap_show(NodeHeap* heap)
void heap_show(NodeArray* heap)
{
    int height = heap_height(heap);

    for (int layer = 0; layer < height; layer++)
    {
        int max_n = int_pow(2, layer);
        int spacing = int_pow(2, height - layer);

        for (int i = 0; i < max_n; i++)
        {
            int arr_i = i + int_pow(2, layer) - 1;
            
            if (arr_i >= heap->len) continue;

            for (int k = 0; k < spacing; k++)
                printf(" ");
            
            printf("%d", heap->elements[arr_i]->F);
        }

        printf("\n");
    }
}