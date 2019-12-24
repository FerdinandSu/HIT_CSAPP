//#include<stdio.h>
//
//typedef unsigned float_bits;
//
//float FB2F(float_bits source) {
//	float* obj = (float*)&source;
//	return *obj;
//}
//float_bits F2FB(float source) {
//	float_bits* obj = (float_bits*)&source;
//	return *obj;
//}
//float_bits float_half(float_bits f) {
//	unsigned sign = f >> 31;
//	unsigned exp = f >> 23 & 0xFF;
//	unsigned frac = f & 0x7FFFFF;
//	if (exp == 0xFF) {
//		return f;
//	}
//	else if(!exp)
//	{
//		if (frac & 1) {
//			frac >>= 1;
//			if (frac & 1) frac++;
//		}
//		else
//		{
//			frac >>= 1;
//		}
//		return frac +( sign << 31);
//	}
//	else if (exp == 1) {
//		if (frac & 1) {
//			frac >>= 1;
//			if (frac & 1) frac++;
//		}
//		else
//		{
//			frac >>= 1;
//		}
//		return frac + ( 1u << 22) +( sign << 31);
//	}
//	else {
//		return frac + (--exp << 23) + (sign << 31);
//	}
//}
//
//main() {
//	FILE* f = fopen("D:\\HW1.log", "w");
//	int WA = 0;
//	float_bits fr = float_half(0u);
//	if (fr != F2FB(0.0 * 0.5)) {
//		WA++;
//		fprintf(f, "%u\t%u\t%u\t0\t%d\n", F2FB(0 / 2.0), F2FB(0 * 0.5), fr, WA);
//		//printf("%f\t%f\t%f\t0\t%d\n", 0.0 / 2.0, 0.0 * 0.5, FB2F(fr), WA);
//	}
//
//
//	for (unsigned i = 1; i != 0; i++)
//	{
//		fr = float_half(i);
//		float fi = FB2F(i);
//		if (fr != F2FB(fi * 0.5f) &&  (i >> 23 != 0xFF)) {
//			WA++;
//			
//			fprintf(f, "%u\t%u\t%u\t%d\t%d\n", F2FB(fi / 2.0), F2FB(fi * 0.5), fr, i, WA);
//			fclose(f);
//			return 1;
//			//printf(f, "%f\t%f\t%f\t%d\t%d\n", i / 2.0, i * 0.5, FB2F(fr), i, WA);
//		}
//		if (!(i&0xffffff)) {
//			printf("%u\n",i);
//		}
//	}
//	fclose(f);
//	return 0;
//}