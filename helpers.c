// helpers.c : utility functions to distribute an interval among workers + more helper functions
// by nicholas raffone and juan piñeros

#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>

// these two functions return an int** [times], each element is an int* [2] with lower and upper bounds for each subinterval [x,y] included
// if the range to be divided is less than the number of intervals requested, the division is done on a smaller interval, while all other intervals are allocated empty [0,0] intervals

int** divide_interval_random(int lower, int upper, int times)
{
    // allocate struct
    int** intervals = (int**) malloc(times*sizeof(int*));
    for(int i = 0; i < times; i++)
    {
        intervals[i] = (int*) malloc(2*sizeof(int));
    }

    // approach: for an interval [x,y] generate for each subinterval a random point x <= k <= y and assign to said subinterval the range [x,k]
    //           for the remaining subintervals repeat the process on [k+1,y]
    //           stop when: k = y therefore we run out of numbers, in that case assign [0,0] to all remaining intervals
    //                      we reach last subinterval: assign [k+1, y]

    int current = lower;
    for(int i = 0; i < times; i++)
    {
        // stopping conditions
        if(current-1 == upper)
        {
            for(int j = i; j < times; j++)
            {
                intervals[j][0] = 0;
                intervals[j][1] = 0;
            }
            break;
        }
        else if(i == times-1)
        {
            intervals[i][0] = current;
            intervals[i][1] = upper;
        }
        // normal case
        else
        {
            int rand_n = (rand() % (upper - current + 1)) + current; // from https://www.includehelp.com/c-programs/generate-random-numbers-within-a-range.aspx
            intervals[i][0] = current;
            intervals[i][1] = rand_n;
            current = rand_n + 1;
        }
    }

    return intervals;
}


int** divide_interval_equally(int lower, int upper, int times)
{
    // allocate struct
    int** intervals = (int**) malloc(times*sizeof(int*));
    for(int i = 0; i < times; i++)
    {
        intervals[i] = (int*) malloc(2*sizeof(int));
    }

    if(upper-lower+1 < times) //upper+lower-1 is the number of elements to be distributed
    {
        // divides the interval equally on the largest possible number of subintervals, this is, 1 per subinterval
        int** intervals_possible = divide_interval_equally(lower, upper, upper-lower+1);
        // up to the intervals possible, copy the answer we got from the last part into our return struct
        for(int i = 0; i < upper-lower+1; i++)
        {
            intervals[i] = intervals_possible[i];
        }
        for(int i = upper-lower+1; i < times; i++)
        {
            // this [0,0] token indicates a worker or delegator to do no work
            intervals[i][0] = 0;
            intervals[i][1] = 0;
        }
    }
    else if(upper-lower+1 == times)
    {
        // divide equally per each subinterval, one each
        for(int i = 0; i < times; i++)
        {
            intervals[i][0] = lower+i;
            intervals[i][1] = lower+i;
        }
    }
    else
    {
        // we divide equally to the best of our capacity
        // this is, we'll divide the intervals equally using integer division and distribute the residue one by one starting by the first interval

        // approach: 1. perform integer division, get the base number of elements (base) per each interval
        //           2. get the residue
        //           3. 0 <= residue < n. For iterations 0 < residue, make a subinterval of base+1 elements. For the rest, make a subinterval of base elements.

        int elements = upper-lower+1;
        int base = elements / times;
        int residue = elements % times;
        int current = lower;
        
        for(int i = 0; i < residue; i++)
        {
            intervals[i][0] = current;
            intervals[i][1] = current+base; // (base+1) elements total
            current = current+base+1; // next element to start 
        }
        for(int i = residue; i < times; i++)
        {
            intervals[i][0] = current;
            intervals[i][1] = current+base-1; // (base) elements total
            current = current+base; // next element to start 
        }
    }

    return intervals;
}

int int_comparer(const void * a, const void * b) // from https://www.tutorialspoint.com/c_standard_library/c_function_qsort.htm
{
    return ( *(int*)a - *(int*)b );
}
