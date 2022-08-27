#include "minheap.h"
// #include "util.h"

// heap functions //

// // n is heap capacity when no init array (NULL) is given
// NodeHeap* heap_init(Node** init_arr, int n)
// {
//     NodeHeap* heap = (NodeHeap*) calloc(1, sizeof(NodeHeap));

//     // no init array -> empty heap with n as capacity
//     if (init_narr == NULL)
//     {
//         heap->elements = (Node**) calloc(caplen, sizeof(Node*));
//         heap->len = 0;
//         heap->cap = n;

//         return heap;
//     }

//     // copy init array to new heap with 2x capacity

//     heap->elements = (Node**) calloc(2 * n, sizeof(Node*));
//     heap->len = n;
//     heap->cap = 2 * n;

//     for (int i = 0; i < n; i++)
//         heap->elements[i] = init_arr[i];

//     return heap;
// }

// // does not free the nodes
// void heap_destroy(NodeHeap* heap)
// {
//     free(heap->elements);
//     free(heap);
// }

// // does free the nodes
// void heap_obliterate(NodeHeap* heap)
// {
//     for (int i = 0; i < heap->len; i++)
//         free(heap->elements[i]);

//     heap_destroy(heap);
// }

// void heap_sortup(NodeHeap* heap, int i)
void heap_sortup(NodeArray* heap, int i)
{
    if (i <= 0) return;

    // parent index
    int ip = (i - 1) / 2;

    if (heap->elements[i]->F >= heap->elements[ip]->F)
    {
        if (heap->elements[i]->F > heap->elements[ip]->F) return;

        if (heap->elements[i]->H > heap->elements[ip]->H) return;
    }

    swap(heap->elements[i], heap->elements[ip]);
    heap_sortup(heap, ip);
}

// void heap_sortdown(NodeHeap* heap)
void heap_sortdown(NodeArray* heap)
{
    int height = heap_height(heap);
    int n = heap->len - ipow(2, height - 1) + 1;

    for (int e = height - 1; e > 0; e--)
    {
        int start = ipow(2, e) - 1;
        
        for (int k = start; k < start + n; k++)
        {
            if (k % 2 == 0) continue;

            int ip = (k - 1) / 2;
            int ir = 2 * ip + 2;

            if (less_expensive(heap, k, ir))
            {
                if (less_expensive(heap, k, ip))
                    swap(heap->elements[k], heap->elements[ip]);
                
                continue;
            }

            if (less_expensive(heap, ir, ip))
                swap(heap->elements[ir], heap->elements[ip]);
        }

        n = ipow(2, e - 1);
    }
}

// void heap_add(NodeHeap* heap, Node* node)
void heap_add(NodeArray* heap, Node* node)
{
    if (heap->len == heap->cap)
        narr_expand(heap);

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
    if (heap->elements[i]->H > heap->elements[k]->H) return 1;
    return 0;
}

// void heap_show(NodeHeap* heap)
void heap_show(NodeArray* heap)
{
    int height = heap_height(heap);

    for (int layer = 0; layer < height; layer++)
    {
        int max_n = ipow(2, layer);
        int spacing = ipow(2, height - layer);

        for (int i = 0; i < max_n; i++)
        {
            int arr_i = i + ipow(2, layer) - 1;
            
            if (arr_i >= heap->len) continue;

            for (int k = 0; k < spacing; k++)
                printf(" ");
            
            printf("%d", heap->elements[arr_i]->F);
        }

        printf("\n");
    }
}

// TESTING //
// #include <time.h>

// int main(void)
// {
//     NodeHeap* test_heap = narr_init(NULL, 20);

//     srand((unsigned) time(NULL));

//     for (int i = 0; i < 10; i++)
//     {
//         Node* test_node = (Node*) calloc(1, sizeof(Node));
//         test_node->F = rand() % 500;
//         heap_add(test_heap, test_node);
//     }
//     heap_show(test_heap);

//     narr_obliterate(test_heap);

//     return 0;
// }