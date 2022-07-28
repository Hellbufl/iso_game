#include "minheap.h"
#include <time.h>

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

// heap functions //

// caplen is capacity or length depending on if the array pointer is null or not
Heap* heap_init(Node** init_array, int caplen)
{
    Heap* heap = (Heap*) calloc(1, sizeof(Heap));

    // no initializer array -> empty heap with caplen as capacity
    if (init_array == NULL)
    {
        heap->elements = (Node**) calloc(caplen, sizeof(Node*));
        heap->len = 0;
        heap->capacity = caplen;

        return heap;
    }

    // copy init array to new heap with 2*caplen as capacity

    heap->elements = (Node**) calloc(2 * caplen, sizeof(Node*));
    heap->len = caplen;
    heap->capacity = 2 * caplen;

    for (int i = 0; i < caplen; i++)
        heap->elements[i] = init_array[i];

    return heap;
}

void heap_destroy(Heap* heap)
{
    free(heap->elements);
    free(heap);
}

void heap_obliterate(Heap* heap)
{
    for (int i = 0; i < heap->len; i++)
        free(heap->elements[i]);
    heap_destroy(heap);
}

void heap_sortup(Heap* heap, int i)
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

void heap_sortdown(Heap* heap)
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

void heap_add(Heap* heap, Node* node)
{
    if (heap->len == heap->capacity)
    {
        Heap* new_heap = heap_init(heap->elements, heap->len);
        heap_destroy(heap);
        heap = new_heap;
    }

    heap->elements[heap->len] = node;
    heap->len += 1;

    heap_sortup(heap, heap->len - 1);
}

Node* heap_take(Heap* heap)
{
    if (heap->len == 0) return NULL;

    Node* first = heap->elements[0];

    for (int i = 0; i < heap->len - 1; i++)
        heap->elements[i] = heap->elements[i+1];
    
    heap->elements[heap->len - 1] = NULL;
    heap->len -= 1;

    return first;
}

int heap_height(Heap* heap)
{
    if (heap->len == 0) return 0;

    // log2(x) = logy(x) / logy(2)
    return (int) (log((double) heap->len) / log(2.0)) + 1;
}

// void heap_show(Heap* heap)
// {
//     int height = heap_height(heap);

//     if(height > 1)
//         printf("%d \n", heap->elements[0]->F);

//     for(int i = 0; i < height - 2; i++)
//     {
//         int indx = ipow(2, i + 1);

//         for(int j = 0; j < indx; j++)
//         {
//             printf("%d ", heap->elements[indx + j -1]->F);
//         }

//         printf("\n");
//     }
// }

void heap_show(Heap* heap)
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

int main(void)
{
    Heap* test_heap = heap_init(NULL, 20);

    srand((unsigned) time(NULL));

    for (int i = 0; i < 10; i++)
    {
        Node* test_node = (Node*) calloc(1, sizeof(Node));
        test_node->F = rand() % 500;
        heap_add(test_heap, test_node);
    }
    heap_show(test_heap);

    heap_obliterate(test_heap);

    return 0;
}