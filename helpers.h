#ifndef HELPERS
#define HELPERS

// helper functions
// these return an array of int* where arr[0] is lower bound, arr[1] is upper bound
int** divide_interval_random(int lower, int upper, int times); //TODO

int** divide_interval_equally(int lower, int upper, int times);

int int_comparer(const void * a, const void * b);

#endif 
