#include<stdio.h>

#define P(A) printf(#A);printf(" %d\n", sizeof(A))

main() {
	P(char);

	P(short);
	P(int);
	P(long);
	P(long long);
	P(float);
	P(double);
	P(long double);
	P(void*);
	return 0;

}