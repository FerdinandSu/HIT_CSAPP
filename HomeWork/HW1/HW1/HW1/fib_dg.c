#include<memory.h>
#include<stdio.h>


typedef int targ_t;
targ_t fib[10000000];

targ_t Fib(int x) {
	if (!fib[x]) 
	{
		fib[x] = Fib(x - 1) + Fib(x - 2);
		printf("Fib[%d]=%d\n", x, fib[x]);
	}
	return fib[x];
}

main() {
	memset(fib, 0, sizeof fib);
	fib[0] = 1;
	fib[1] = 1;
	Fib(10000000);
}