#define MAX(a, b) ((a) * ((a) >= (b)) + (b) * ((b) > (a)))
#define MIN(a, b) ((a) * ((a) <= (b)) + (b) * ((b) < (a)))

float fast_isqrt(float x);
int int_pow(int b, int e);
int int_abs(int x);
float float_abs(float x);
int float_sign(float x);