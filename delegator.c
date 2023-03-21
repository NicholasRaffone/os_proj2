// delegator.c : delegator node of an asynchronous prime number calculator
// by nicholas raffone and juan piñeros

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "helpers.h"
#include <unistd.h>
#include <signal.h>
#include <string.h>

void handle_usrsig1();
void handle_usrsig2();

int main(int argc, char** argv)
{
    signal(SIGUSR1, handle_usrsig1);
    signal(SIGUSR2, handle_usrsig2);
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

    // 2. open pipes and distribute to each worker -> this is basically the same as in primes.c.
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
            // from requirements: worker.o -l x -u y -a (1 or 2)
            char* arguments[8];
            for(int j = 0; j < 8; j++)
            {
                arguments[j] = malloc(32*sizeof(char));
            }
            strcpy(arguments[0], "./worker");
            //bounds
            strcpy(arguments[1], "-l");
            sprintf(arguments[2], "%d", intervals[i][0]);
            strcpy(arguments[3], "-u");
            sprintf(arguments[4], "%d", intervals[i][1]);
            // algorithm (1 if odd, 2 if even)
            strcpy(arguments[5], "-a");
            sprintf(arguments[6], "%d", algorithm);

            arguments[7] = NULL;

            // make the pipe to parent stdin and out of the child program by dup2()ing it
            dup2(pipes[i][0], 0); //set stdin of child program to read end of pipe 
            dup2(pipes[i][1], 1); //same for stdout
            close(pipes[i][0]);
            close(pipes[i][1]);
            // exec it
            execv("worker", arguments);
            for(int j = 0; j < 8; j++){
                free(arguments[j]);
            }
        }
        // if parent, add to children_pid
        else
        {
            children_pid[i] = pid;
        }
    }

    // 3. wait for workers to finish, then look at pipe info and aggregate it
    int total_primes = 0;
    int* curr_primes = (int*) malloc(total_primes*sizeof(int));
    int* all_times = malloc(nodes*sizeof(int));
    // loop of waits - do as in primes.c
    for(int i = 0; i < nodes; i++){
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
            if(number!= 0){
                curr_primes = realloc(curr_primes, (total_primes+1)*sizeof(int));
                curr_primes[total_primes] = number;
                total_primes++;
            }
        }

        int time;
        read(current_pipe[0], &time, sizeof(int));
        all_times[i] = time;
    }
    // 4. send data to stdout

    write(1, &total_primes, sizeof(int));
    for(int i = 0; i<total_primes; i++){
        write(1, &curr_primes[i], sizeof(int));
    }
    free(curr_primes);

    write(1, &nodes, sizeof(int));
    for(int i = 0; i<nodes; i++){
        write(1, &all_times[i], sizeof(int));
    }
    free(all_times);

    for(int i = 0; i < nodes; i++){
        free(pipes[i]);
    }
    free(pipes);
    free(children_pid);

    // exit
    return 0;
}

// pass signals to parent
void handle_usrsig1()
{
    signal(SIGUSR1, handle_usrsig1);
    kill(getppid(), SIGUSR1);
}
void handle_usrsig2()
{
    signal(SIGUSR2, handle_usrsig2);
    kill(getppid(), SIGUSR2);
}
