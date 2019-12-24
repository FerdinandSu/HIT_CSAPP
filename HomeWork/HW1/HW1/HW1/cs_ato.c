#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define Eps 1e-6



unsigned cs_atoi(char* c) {

	for (unsigned r = 0;;c++) {
		if (!*c)
			return r;
		else if (*c >= '0' && *c <= '9')
			r = (r << 3) + (r << 1) + *c - '0';
	}
}

double cs_atof(char* c) {
	double r = 0, m = 1e1;
	int flag = 0, is_minus=0;
	for (;; c++) {
		if (!*c)
			return is_minus ? -r : r;
		else if (*c == '.')
			flag = 1;
		else if (*c == '-')
			is_minus = 1;
		else if (*c >= '0' && *c <= '9') {
			if (flag)
				r += (*c - '0') / m, m *= 10;
			else
				r = r * 10 + *c - '0';
		}
	}
}

void cs_itoa(unsigned i, char* t){
	const char* stk = malloc(16 * sizeof(char));
	char* stkptr = stk;
	for (;i;i /= 10)
		*++stkptr = i % 10;
	for (;stkptr != stk;t++, stkptr--)
		*t = *stkptr + '0';
	free(stk);
}

typedef unsigned long long UInt64;
typedef unsigned char Byte;

Byte mul2[10] = { 0, 2, 4, 6, 8, 0, 2, 4, 6, 8 };

typedef struct Decimal_t {
	unsigned short Size;
	Byte Digit[310];
} Decimal;


static inline void Decimal_mul_2(Decimal* d) {
	Byte* flag = malloc((d->Size) * sizeof(Byte));
	memset(flag, 0, (d->Size) * sizeof(Byte));
	int i = 0;
	for (; i < d->Size; i++)
	{
		d->Digit[i] = mul2[d->Digit[i]];
		flag[i] = (d->Digit[i] > 4);
		if (i && flag[i - 1]) d->Digit[i]++;
	}
	if (flag[i - 1]) d->Digit[i]++, d->Size++;
}

static inline char* print_UInt64(UInt64 i, char* t) {
	const char* stk = malloc(21 * sizeof(char));
	char* stkptr = stk;
	for (; i; i /= 10)
		*++stkptr = i % 10;
	for (; stkptr != stk; t++, stkptr--)
		*t = *stkptr + '0';
	free(stk);
	return t - 1;
}

static inline char* print_UInt64_6(UInt64 i, char* t) {
	const char* stk = malloc(21 * sizeof(char));
	char* stkptr = stk;
	for (int j = 0; i || j < 6; i /= 10, j++)
		*++stkptr = i % 10;
	for (; stkptr != stk; t++, stkptr--)
		*t = *stkptr + '0';
	free(stk);
	return t - 1;
}

void cs_ftoa(double f, const char* t) {
	UInt64 uf = *(UInt64*)&f;
	UInt64 sign = uf >> 63;
	UInt64 exp = uf >> 52 & 0x7FF;
	UInt64 frac = uf & 0xFFFFFFFFFFFFF;

	char* wrptr = t;

	if (exp == 0x7FF) {
		if (frac) {
			*wrptr = "NaN";
		}
		else
		{
			if (sign)
			{
				*wrptr = "-inf";
			}
			else
			{
				*wrptr = "inf";
			}
		}
	}
	else if(exp > 1075ull)//if this is a big int [1023+52]
	{
		Decimal* d = malloc(sizeof(Decimal));
		memset(d, 0, sizeof(Decimal));
		UInt64 i = frac + (1ull << 52);
		for (; i; i /= 10)
			d->Digit[d->Size++] = i % 10;
		for (; exp > 1075ull; exp--)
			Decimal_mul_2(d);
		char* stkptr = &(d -> Digit[d->Size - 1]);
		if (sign) *wrptr = '-', wrptr++;
		for (; stkptr >= d->Digit; wrptr++, stkptr--)
			*wrptr = *stkptr + '0';
		*wrptr = '.', wrptr++;
		for (int six = 0; six < 6; six++)
			*wrptr = '0', wrptr++;
		*wrptr = 0;
		free(d);
	}
	else
	{
		if (sign) f = -f;
		UInt64 int_part = (UInt64)f;
		UInt64 dec_part = (UInt64)((f - (double)int_part) * 1e7);
		UInt64 dec_part_end = dec_part % 10;
		dec_part /= 10;
		if (dec_part_end >= 6ull || 
			(dec_part_end == 5ull && (dec_part & 1))) {
			dec_part++;
		}
		if (sign) *wrptr = '-', wrptr++;
		wrptr = print_UInt64(int_part, wrptr);
		*wrptr = '.', wrptr++;
		print_UInt64_6(dec_part, wrptr);

	}
}


char t[100];
char td[400];
int main() {

	unsigned D;
	double f;

	for (;;) {
		scanf("%lf", &f);
		f = (double)0x8FFFFFFFFFFFFF;
		char* wrptr = td;
		UInt64 uf = *(UInt64*)&f;
		UInt64 sign = uf >> 63;
		UInt64 exp = uf >> 52 & 2047ull;
		UInt64 frac = uf & 4503599627370495ull;

		Decimal* d = malloc(sizeof(Decimal));
		memset(d, 0, sizeof(Decimal));
		UInt64 i = frac + (1ull << 52);
		for (; i; i /= 10)
			d->Digit[d->Size++] = i % 10;
		for (; exp > 1075ull; exp--)
			Decimal_mul_2(d);
		char* stkptr = &(d->Digit[d->Size - 1]);
		if (sign) *wrptr = '-', wrptr++;
		for (; stkptr >= d->Digit; wrptr++, stkptr--)
			*wrptr = *stkptr + '0';
		printf("%s\n", td);
		*wrptr = '.', wrptr++;
		for (int six = 0; six < 6; six++)
			*wrptr = '0', wrptr++;
		printf("%s\n", td);
		*wrptr = 0;
	}



	for (;;) {
		memset(t, 0, sizeof(t));
		memset(td, 0, sizeof(td));
		scanf("%s", t);
		D = cs_atoi(t);
		f = cs_atof(t);
		printf("%d\n", D);
		printf("%f\n", f);
		cs_itoa(D, t);
		cs_ftoa(f, td);
		printf("%s\n%s\n", t, td);
	}

	printf("%f %lld %d", 1e308, (long long)13.3, (int)-12.1);
	return 0;
}