all: primes

clean:
	rm -f primes.o delegator.o worker.o helpers.o prime_algorithms.o primes delegator worker

primes: helpers.o prime_algorithms.o primes.o delegator worker.o 
	gcc primes.o helpers.o prime_algorithms.o -o primes -lm && gcc delegator.o helpers.o prime_algorithms.o -o delegator -lm && gcc worker.o prime_algorithms.o -o worker -lm

primes.o: primes.c primes.h helpers.o
	gcc -c primes.c

delegator: delegator.o helpers.o
	gcc delegator.o helpers.o prime_algorithms.o -o delegator -lm

delegator.o: delegator.c delegator.h helpers.o
	gcc -c delegator.c

worker: worker.o prime_algorithms.o
	gcc worker.o prime_algorithms.o -o worker -lm

worker.o: worker.c prime_algorithms.h
	gcc -c worker.c

helpers.o: helpers.c helpers.h
	gcc -c helpers.c

prime_algorithms.o: prime_algorithms.c prime_algorithms.h
	gcc -c prime_algorithms.c -lm

