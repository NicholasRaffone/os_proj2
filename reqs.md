### Requirements
- primes.c
    primes -l lower -u upper -[e|r] -n nodes
    1. distribute the interval bw nodes
    2. for n in nodes
        opens a pipe
        forks a child
        child executes delegator.o parameter -l x -u y -e|r -n nodes -a (1 or 2) -b pipenumber (need this to know which pipe to write to from)
    
    3. for every child
        loop of waitpids
        (the last thing a delegator does is to send data through pipe)
        ##### format of message: prime prime prime (..) \n time time time time
        when waitpid is done for child: read from pipe
    when all children are done: print primes

    4. somewhere: have a running counter of signals **STILL NOT DONE**
        4.1. sort primes

    5. p r i n t f ( ) min, max, avg time, signals

- delegator.c -l x -u y -e|r -n nodes -a (1 or 2) -b pipenumber (need this to know which pipe to write to from)
    1. distribute the interval given
    2. for n in nodes
        pens a pipe
        forks a child
        child executes worker.o parameter -l x -u y

    3. for every child
        loop of waitpids
        (the last thing a worker does is to send data through pipe)
        ##### format of message: prime prime prime (..) \n time time time time

        child execs worker.c -l x -u y -a (1 or 2)
    
    4. aggregate all primes
    5. aggregate all times
    6. send data through pipe **format of message: prime prime prime (..) \n time time time time**
    7. exit

- worker.c -l x -u y -a (1 or 2)
    1. start to measure time
    2. execute algo 1 or 2,
    3. end time
    4. send data through pipe **format of message: prime prime prime (..) \n time**
    5. send sig1 or sig2
    6. exit
