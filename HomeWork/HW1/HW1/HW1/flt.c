#include<stdio.h>

typedef unsigned float_bits;

float FB2F(float_bits source) {
	float* obj = (float*)&source;
	return *obj;
}
float_bits F2FB(float source) {
	float_bits* obj = (float_bits*)&source;
	return *obj;
}
float_bits float_half(float_bits f) {
	unsigned sign = f >> 31;
	unsigned exp = f >> 23 & 0xFF;
	unsigned frac = f & 0x7FFFFF;
	if (exp == 0xFF) {
		return f;
	}
	else if(!exp)
	{
		if (frac & 1) {
			frac >>= 1;
			if (frac & 1) frac++;
		}
		else
		{
			frac >>= 1;
		}
		return frac +( sign << 31);
	}
	else if (exp == 1) {
		if (frac & 1) {
			frac >>= 1;
			if (frac & 1) frac++;
		}
		else
		{
			frac >>= 1;
		}
		return frac + ( 1u << 22) +( sign << 31);
	}
	else {
		return frac + (--exp << 23) + (sign << 31);
	}
}

main() {
	printf("%.039f\n", FB2F((unsigned)0x007fffff));
	printf("%f\n", FB2F((unsigned)0x7f7fffff) - FB2F((unsigned)0x7f000000));

	return 0;
}