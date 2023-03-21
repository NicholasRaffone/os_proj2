### Requirements
- primes.c
    primes -l lower -u upper -[e|r] -n nodes 
    1. distribute the interval bw nodes 
    2. for n in nodes 
        opens a pipe
        forks a child
        dup2() pipe ends to stdin and stdout 
        child executes delegator.o parameter -l x -u y -e|r -n nodes -a (1 or 2)
    
    3. for every child 
        loop of waitpids
        (the last thing a delegator does is to send data through pipe)
        ##### format of message: num_primes prime prime prime (..) num_times time time time time
        when waitpid is done for child: read from pipe
    when all children are done: print primes

    4. somewhere: have a running counter of signals 
        4.1. sort primes

    5. p r i n t f ( ) min, max, avg time, signals 

- delegator.c -l x -u y -e|r -n nodes -a (1 or 2)
    1. distribute the interval given 
    2. for n in nodes 
        opens a pipe
        forks a child
        dup2() pipe ends to stdin and stdout
        child executes worker.o parameter -l x -u y

    3. for every child 
        loop of waitpids
        (the last thing a worker does is to send data through pipe)
        ##### format of message: num_primes prime prime prime (..) time 

        child execs worker.c -l x -u y -a (1 or 2)
    
    4. aggregate all primes
    5. aggregate all times
    6. send data through pipe format of message: 
        ##### num_primes prime prime prime (..) num_times time time time time aka send data to stdout
    7. exit

- worker.c -l x -u y -a (1 or 2)
    1. start to measure time
    2. execute algo 1 or 2,
    3. end time
    4. send data through pipe **format of message: num_primes prime prime prime (..) time** aka send data to stdout
    5. send sig1 or sig2
    6. exit

- helper functions
    1. divide an interval randomly 
    2. divide an interval equally 