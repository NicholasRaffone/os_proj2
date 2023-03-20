// primes.c : root node of an asynchronous prime number calculator
// by nicholas raffone and juan piñeros

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "helpers.h"
#include <unistd.h>



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
    else if (distributing == 'e')
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
        // if child, exec() stuff
        if(pid == 0)
        {
            // prepare arguments
            // from requirements: delegator.o -l x -u y -e|r -n nodes -a (algorithm 1 or 2) -b pipenumber 
            char* arguments[13];
            for(int j = 0; j < 12; j++)
            {
                arguments[j] = malloc(32*sizeof(char));
            }
            strcpy(arguments[0], "delegator.o");
            //bounds
            strcpy(arguments[1], "-l");
            sprintf(arguments[2], "%d", intervals[i][0]);
            strcpy(arguments[3], "-u");
            sprintf(arguments[4], "%d", intervals[i][1]);
            // random or equal
            if (distributing == 'r')
            {
                strcpy(arguments[5], "-r");
            }
            else if (distributing == 'e')
            {
                strcpy(arguments[5], "-e");
            }
            // number of workers
            strcpy(arguments[6], "-n");
            sprintf(arguments[7], "%d", nodes);
            // algorithm (1 if odd, 2 if even)
            strcpy(arguments[8], "-a");
            if(i%2 != 0)
            {
                strcpy(arguments[9], "1");
            }
            else
            {
                strcpy(arguments[9], "2");
            }
            // pipe number
            strcpy(arguments[10], "-b");
            sprintf(arguments[11], "%d", i);

            arguments[12] = NULL;

            // exec it
            execv("delegator.o", arguments); 
        }
        // if parent, add to children_pid
        else
        {
            children_pid[i] = pid;
        }
    }

    // 3. wait for delegators to finish, then look at pipe info and aggregate it
    char** all_primes_received = malloc(nodes*sizeof(char*)); // stores all strings received from delegators containing calculated primes
    char** all_times_received - malloc(nodes*sizeof(char*)); // same, but for times

    // loop of waits
    for(int i = 0; i < nodes; i)
    {
        int status;
        waitpid(children_pid[i], &status, 0);

        // look at pipe info
        int* current_pipe = pipes[i];
        // close write end
        close(current_pipe[1]);
        // we prepare to read an insanely large number of bytes
        char* primes_from_child[1000];
        char* times_from_child[1000];

        // format of message: prime prime prime (..) \n time time time time
        // first line: primes
        fgets(primes_from_child, 1000, current_pipe[0]);
        // second line: times
        fgets(times_from_child, 1000, current_pipe[0]);

        // add to aggregators
        all_primes_received[i] = primes_from_child;
        all_times_received[i] = times_from_child;
    }

    // 4. get an array of all primes and all times
    int prime_size;
    int time_size;
    int* primes = ints_from_group_of_strings(all_primes_received, &prime_size);
    int* times = ints_from_group_of_strings(all_times_received, &time_size);

    qsort()
    
    // 5. output results
    printf("All primes found: \n");
    for(int i = 0; i < prime_size; i++)
    {
        printf("%d ", primes[i]);
    }
    printf("\n");


    return 0;
}


