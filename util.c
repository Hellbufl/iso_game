#include "util.h"

// utility functions //

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