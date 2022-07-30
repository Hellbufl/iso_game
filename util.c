#include "util.h"
#include "node.h"
#include "minheap.h"

// utility functions //

float fast_isqrt(float x)
{
    int i;
    float x2, y;
    const float threehalfs = 1.5f;

    x2 = x * 0.5f;
    y = x;
    i = * (int*) &y;
    i = 0x5f3759df - (i >> 1);
    y = * (float*) &i;
    y = y * (threehalfs - (x2 * y * y));
    // y = y * (threehalfs - (x2 * y * y)); //2nd iteration
    
    return y;
}

int ipow(int b, int e)
{
    int result = 1;

    for (int i = 0; i < e; i++)
        result *= b;

    return result;

}

float float_abs(float x)
{
    int ix = *((int*) &x) & 0x7FFFFFFF;
    return *((float*) &ix);
}

int dsign(float x)
{
    return -((*((int*) &x) & 0x80000000) / 0x40000000) + 1;
}

void swap(Node* e1, Node* e2)
{
    Node tmp = *e1;
    *e1 = *e2;
    *e2 = tmp;
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