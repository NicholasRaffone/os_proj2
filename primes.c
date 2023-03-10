// primes.c : root node of an asynchronous prime number calculator
// by nicholas raffone and juan piñeros

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


// helper functions
// these return an array of int* where arr[0] is lower bound, arr[1] is upper bound
int** divide_interval_random(int lower, int upper, int times);

int** divide_interval_equally(int lower, int upper, int times);

// primes -l lower -u upper -[e|r] -n nodes
int main(int argc, char** argv)
{
    int lower;
    int upper;
    char distributing; 
    int nodes; 
    int c;

    // parse commandline arguments (inspired by https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html)
    while ((c = getopt (argc, argv, "l:u:ern:")) != -1)
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
            default:
                printf("One or more flags not recognised. Exiting...\n");
                exit(0);
        }
    printf("%d %d %c %d\n", lower, upper, distributing, nodes);

    // 1. assign intervals
    int** intervals;
    if (distributing == 'r')
    {
        intervals = divide_interval_random(lower, upper, nodes);
    }
    if (distributing == 'e')
    {
        intervals = divide_interval_equally(lower, upper, nodes);
    }

    // 2. open pipes and distribute to each thing 
    int** pipes = malloc(sizeof(int*) * nodes);
    pid_t* children_pid = malloc(sizeof(pid_t) * nodes);
    
    for(int i = 0; i < nodes; i++)
    {
        pipe(pipes[i]);
        pid_t pid = fork();
        // if parent, add to children_pid

        // if child, exec() stuff

    }
    


    return 0;
}

