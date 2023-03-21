# Primes (CS-UH 3010 Project 2)
### Created by Juan Piñeros and Nicholas Raffone

#### The primes program allows one to get the primes between 2 values using forks and pipes.

## Usage

### Compilation
Run the command `make` to assemble all files

### Running the program 
Run `./primes -l LowerBound -u UpperBound -[e|r] -n NumOfNodes` to execute the program

### Removing files
Run the command `make clean` to delete all files

## Design Choices

The overall structure of the program flow is as follows: Primes->Delegators->Workers. The `primes` program intially accepts parameters detailing how the program should execute.

The data is then sent downards through command line arguments in the format `delegator.c -l x -u y -e|r -n nodes -a {1|2}`, where the bounds are divided between the multiple delegators by the prime program. The commandline arguments are further divided by the delegator to the worker, where the worker is executed by the command `worker.c -l x -u y -a {1|2}`. The worker then computes the necessary primes and sends them to the delegator via a pipe.

The pipe is created by the prime and delegator to send data back to them. The pipe's file descriptor is set to the standard input by the dup2 method and is read in the format `{num_primes} {...primes} {num_times} {...times}` in the case of data to the delegator with multiple times, and `{num_primes} {...primes} {...times}` in the case of the worker with one singular time to report.

The worker records the time it takes to execute, and upon completion, send SIGUSR1 or SIGUSR2 depending on the algorithm it completed to its parent. The delegator reads this signal and passes it up to the main program. 

Once the main program has all of the necessary data, it aggregates the data using the built-in qsort method to find the average of primes and times, then also finds the min, max, etc for the times.
