#ifndef DIVIDE_INTERVALS
#define DIVIDE_INTERVALS

// helper functions
// these return an array of int* where arr[0] is lower bound, arr[1] is upper bound
int** divide_interval_random(int lower, int upper, int times);

int** divide_interval_equally(int lower, int upper, int times);

#endif 