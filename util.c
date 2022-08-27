#include "util.h"
// #include "node.h"
// #include "minheap.h"

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