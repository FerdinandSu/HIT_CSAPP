#include<stdlib.h>
#include<string.h>

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
	else if (exp > 1075ull)//if this is a big int [1023+52]
	{
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