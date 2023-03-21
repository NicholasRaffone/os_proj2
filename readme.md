# Primes (CS-UH 3010 Project 2)
### Created by Juan Piñeros and Nicholas Raffone

#### The primes program allows one to get the primes between 2 values using forks and pipes.

## Usage

### Compilation
Run the command `make` to assemble all files

### Running the program 
Run `./primes -l LowerBound -u UpperBound -[e|r] -n NumOfNodes` to execute the program. The resulting structure will be a full n-ary tree of processes with $n^2$ workers.

### Removing files
Run the command `make clean` to delete all files

## Design Choices

The overall structure of the program flow is as follows: Primes->Delegators->Workers. The `primes` program intially accepts parameters detailing how the program should execute.

The data is then sent downards through command line arguments in the format `delegator.c -l x -u y -e|r -n nodes -a {1|2}`, where the bounds are divided between the multiple delegators by the prime program. The bounds are divided either randomly or equally depending on the `-e` or `-r` flag. These bounds are further divided by the delegator to its associated workers, where the worker is executed by the command `worker.c -l x -u y -a {1|2}`. 

The worker then computes the necessary primes and sends them to the delegator via a pipe. Similarly, having obtained all products from all workers, each delegator sends its data to the prime program.

Additionally, each worker records the time it takes to execute, and upon completion, sends SIGUSR1 or SIGUSR2 depending on the algorithm it completed to its parent. The delegator reads this signal and signals it back to the main program.

Once the main program has all of the necessary data from the delegators, it aggregates the data using the built-in qsort method to find the average of primes and times, then also finds the min, max, etc for the times.

### Interval division

We implement methods that return an array of tuples of `int`s that represent a closed interval, or, in the case of having run out of elements to allocate, a tuple `[0,0]` that signals there is no work to be done. The `divide_interval_randomly` method generates random numbers $lower \leq k \leq upper$ that divide the interval into $n$ parts. The `divide_interval_equally` method divides the interval into $n$ parts using integer division and divides the residue evenly among the first parts.


### Piping conventions
The root process creates a pipe for each delegator. Similarly, each delegator creates a pipe for each worker. In both cases, when a child process is `fork()`ed, the pipe created for it is `dup2()`ed to the standard input and output of the child process. After this is done, the child process executes the appropriate program.

#### Sending data
The workers report the primes they found and the time they took to their delegator in the format `{num_primes} {...primes} {time}`. The delegator then aggregates these messages and sends the data to the prime process in the format `{num_primes} {...primes} {num_times} {...times}`. Both workers and delegators exit immediately after sending their data.

#### Receiving data
Both the root process and the delegators enter a loop of `waitpid()` for all of their children. This means that when a child process exits, the parent process will be notified of a change of state and read the child's pipe for the data requested. 



*For a working draft of the project's outline and pseudocode, see `reqs.md`.*

@juandapl, @NicholasRaffone, 

21/03/2023