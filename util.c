#include "util.h"

// utility functions //

// float fast_isqrt(float x)
// {
//     int i;
//     float x2, y;
//     const float threehalfs = 1.5f;

//     x2 = x * 0.5f;
//     y = x;
//     i = * (int*) &y;
//     i = 0x5f3759df - (i >> 1);
//     y = * (float*) &i;
//     y = y * (threehalfs - (x2 * y * y));
//     y = y * (threehalfs - (x2 * y * y)); //2nd iteration
    
//     return y;
// }

int int_pow(int basis, int exponent)
{
    int result = 1;

    for (int i = 0; i < exponent; i++)
        result *= basis;

    return result;
}

float float_abs(float number)
{
    uint32_t out_number = *((uint32_t*) &number) & 0x7FFFFFFF;
    return *((float*) &out_number);
}

int float_sign(float number)
{
    return -((*((uint32_t*) &number) & 0x80000000) / 0x40000000) + 1;
}