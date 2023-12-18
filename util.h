#include <inttypes.h>
#define MAXOF(a, b) ((a) * ((a) >= (b)) + (b) * ((b) > (a)))
#define MINOF(a, b) ((a) * ((a) <= (b)) + (b) * ((b) < (a)))

int int_pow(int b, int e);
float float_abs(float x);
int float_sign(float x);

// TODO: remove these functions lol