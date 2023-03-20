// delegator.c : delegator node of an asynchronous prime number calculator
// by nicholas raffone and juan piñeros

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "helpers.h"
#include <unistd.h>
#include <signal.h>

int main(int argc, char** argv)
{
    int lower;
    int upper;
    char distributing; 
    int nodes; 
    int algorithm;
    int c;

    // delegator.c -l x -u y -e|r -n nodes -a (1 or 2)
    while ((c = getopt (argc, argv, "l:u:ern:a:")) != -1)
        switch (c)
        {
            case 'l':
                lower = atoi(optarg);
                break;
            case 'u':
                upper = atoi(optarg);
                break;
            case 'e':
                distributing = c;
                break;
            case 'r':
                distributing = c;
                break;
            case 'n':
                nodes = atoi(optarg);
                break;
            case 'a':
                algorithm = atoi(optarg);
                break;
            default:
                printf("One or more flags not recognised. Exiting...\n");
                exit(0);
        }
    
    // 1. assign intervals
    int** intervals;
    if (distributing == 'r')
    {
        intervals = divide_interval_random(lower, upper, nodes);
    }
    else if (distributing == 'e')
    {
        intervals = divide_interval_equally(lower, upper, nodes);
    }

    // 2. 

}