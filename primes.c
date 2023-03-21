// primes.c : root node of an asynchronous prime number calculator
// by nicholas raffone and juan piñeros

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "helpers.h"
#include <unistd.h>
#include <signal.h>
#include <string.h>

int sig1_count = 0;
int sig2_count = 0;

void handle_usrsig1();
void handle_usrsig2();

// primes.o -l lower -u upper -[e|r] -n nodes
int main(int argc, char** argv)
{

    // set behaviour when receiving signals
    signal(SIGUSR1, handle_usrsig1);
    signal(SIGUSR2, handle_usrsig2);

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
        pipes[i] = malloc(2*sizeof(int));
        pipe(pipes[i]);
        pid_t pid = fork();
        // if child, exec() stuff
        if(pid == 0)
        {
            // prepare arguments
            // from requirements: delegator.o -l x -u y -e|r -n nodes -a (algorithm 1 or 2) 
            char* arguments[11];
            for(int j = 0; j < 11; j++)
            {
                arguments[j] = malloc(32*sizeof(char));
            }
            strcpy(arguments[0], "./delegator");
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

            arguments[10] = NULL;

            // make the pipe to parent stdin and out of the child program by dup2()ing it
            dup2(pipes[i][0], 0); //set stdin of child program to read end of pipe 
            dup2(pipes[i][1], 1); //same for stdout
            close(pipes[i][0]);
            close(pipes[i][1]);

            // exec it
            execv("./delegator", arguments); 
            for(int j = 0; j < 11; j++){
                free(arguments[j]);
            }
        }
        // if parent, add to children_pid
        else
        {
            children_pid[i] = pid;
        }
    }

    // 3. wait for delegators to finish, then look at pipe info and aggregate it
    int total_primes = 0;
    int* all_primes = (int*) malloc(total_primes*sizeof(int));

    int total_times = 0;
    int* all_times = malloc(total_times*sizeof(int));
    // loop of waits
    for(int i = 0; i < nodes; i++)
    {
        int status;
        waitpid(children_pid[i], &status, 0);
        // look at pipe info
        int* current_pipe = pipes[i];
        // close write end
        close(current_pipe[1]);
        // we prepare to read an insanely large number of bytes

        int num_nums;
        read(current_pipe[0], &num_nums, sizeof(int));
        int number;
        for(int p=0; p<num_nums; p++){
            read(current_pipe[0], &number, sizeof(number));
            all_primes = realloc(all_primes, (total_primes+1)*sizeof(int));
            all_primes[total_primes] = number;
            total_primes++;
        }

        int num_times;
        read(current_pipe[0], &num_times, sizeof(int));
        int time;
        for(int p=0; p<num_times; p++){
            read(current_pipe[0], &time, sizeof(time));
            all_times = realloc(all_times, (total_times+1)*sizeof(int));
            all_times[total_times] = number;
            total_times++;
        }
    }

    // 4. get an array of all primes and all times
    qsort(all_primes, total_primes, sizeof(int), int_comparer);
    qsort(all_times, total_times, sizeof(int), int_comparer);

    int sum = 0;
    for(int i = 0; i < total_times; i++)
    {
        sum += all_times[i];
    }
    float avg_time = (float) sum / total_times;

    // 5. output results
    printf("All primes found: \n");
    for(int i = 0; i < total_primes; i++)
    {
        printf("%d ", all_primes[i]);
    }
    printf("\n");
    printf("Min. time to finish interval: %.2f\n", (float) all_times[0]/1000);
    printf("Max. time to finish interval: %.2f\n", (float) all_times[total_times-1]/1000);
    printf("Avg. time to finish interval: %.2f\n", avg_time/1000);
    printf("No. of SIGUSR1: %d\n", sig1_count);
    printf("No. of SIGUSR2: %d\n", sig2_count);

    free(all_primes);
    free(all_times);

    for(int i = 0; i < nodes; i++){
        free(pipes[i]);
    }
    free(pipes);
    free(children_pid);

    return 0;
}

void handle_usrsig1()
{
    signal(SIGUSR1, handle_usrsig1);
    sig1_count++;
}
void handle_usrsig2()
{
    signal(SIGUSR2, handle_usrsig2);
    sig2_count++;
}
