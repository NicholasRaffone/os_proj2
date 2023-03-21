// worker.c : leaf node of a distributed asynchronous prime number calculator
// by nicholas raffone and juan piñeros

#include "prime_algorithms.h"
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <signal.h>

int main(int argc, char** argv)
{
    int c;
    int lower;
    int upper;
    int algo;
    double t1, t2, cpu_time; struct tms tb1, tb2; double ticspersec;
    int i, sum = 0;
    ticspersec = (double) sysconf(_SC_CLK_TCK); t1 = (double) times(&tb1);
    
    // parse commandline arguments (inspired by https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html)
    while ((c = getopt (argc, argv, "l:u:a:")) != -1)
        switch (c)
        {
            case 'l':
                lower = atoi(optarg);
                break;
            case 'u':
                upper = atoi(optarg);
                break;
            case 'a':
                algo = atoi(optarg);
                break;
            default:
                printf("One or more flags not recognised. Exiting...\n");
                exit(0);
        }

    int num_nums = 0;
    int* nums = (int*) malloc(num_nums*sizeof(int));
    for(int i = lower; i<=upper; i++){
        if(algo==1 && prime_algo1(i)==1){
            nums = realloc(nums, (num_nums+1)*sizeof(int));
            nums[num_nums] = i;
            num_nums++;
        }else if(algo==2 && prime_algo2(i)==1){
            nums = realloc(nums, (num_nums+1)*sizeof(int));
            nums[num_nums] = i;
            num_nums++;
        }
    }
    write(1, &num_nums, sizeof(int));
    for(int i = 0; i< num_nums; i++){
        write(1, &nums[i], sizeof(int));
    }
    t2 = (double) times(&tb2);
    int total_time = (int) (1000*(t2 - t1) / ticspersec);
    write(1, &total_time, sizeof(int));
    if(algo==1){
        kill(getppid(),SIGUSR1);
    }else{
        kill(getppid(),SIGUSR2);
    }
    free(nums);
    return 0;
}
