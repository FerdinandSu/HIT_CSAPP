#include<memory.h>
#include<stdio.h>


typedef unsigned targ_t;
targ_t fib[10000000];


main() {
	memset(fib, 0, sizeof fib);
	fib[0] = 1;
	fib[1] = 1;
	for (int i = 2; i < 1000000; i++) {
		fib[i] = fib[i - 1] + fib[i - 2];
		printf("Fib[%d]=%u\n", i, fib[i]);
		if (fib[i] < fib[i - 1]) { break; }
	}
}