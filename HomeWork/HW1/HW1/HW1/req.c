#include <stdio.h>


#define MAX 0xFF
int f[MAX];

int fib(int x) {
	if (x == 0) {
		f[0] = 0;
	}
	else if (x == 1) {
		f[1] = 1;
	}
	else {
		f[x] = fib(x - 1) + fib(x - 2);
	}
	return f[x];

}

int main() {
	printf("fib starts\n");
	fib(50);
	printf("fib ends;\n");
	for (int i = 0; i <= 50; i++)
	{
		printf("f[%d] = %d\n", i, f[i]);
	}
	return 0;

}
