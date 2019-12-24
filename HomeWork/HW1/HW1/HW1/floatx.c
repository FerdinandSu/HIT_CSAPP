
#include<stdio.h>
typedef unsigned char byte ;

//+0-0，最小浮点正数，最大浮点正数、最小正的规格化浮点数、正无穷大、Nan

typedef unsigned float_bits;

float FB2F(float_bits source) {
	float* obj = (float*)&source;
	return *obj;
}
float_bits F2FB(float source) {
	float_bits* obj = (float_bits*)&source;
	return *obj;
}
int main() {
	float f[7];
	f[0] = 0.0f;
	f[1] = -0.0f;
	f[2] = FB2F((unsigned)0x00000001);
	f[3] = FB2F((unsigned)0x7f7fffff);
	f[4] = FB2F((unsigned)0x00800000);
	f[5] = FB2F((unsigned)0x7fffffff);
	f[6] = FB2F((unsigned)0x7ffffff1);

	printf("+0\t\t\t\t%08X\t\t\t\t%f\n", F2FB(f[0]), f[0]);
	printf("-0\t\t\t\t%08X\t\t\t\t%f\n", F2FB(f[1]), f[1]);
	printf("最小浮点正数\t\t\t%08X\t\t\t\t%f\n", F2FB(f[2]), f[2]);
	printf("最大浮点正数\t\t\t%08X\t\t\t\t%f\n", F2FB(f[3]), f[3]);
	printf("最小正的规格化浮点数\t\t%08X\t\t\t\t%f\n", F2FB(f[4]), f[4]);
	printf("正无穷大\t\t\t%08X\t\t\t\t%f\n", F2FB(f[5]), f[5]);
	printf("NaN\t\t\t\t%08X\t\t\t\t%f\n", F2FB(f[6]), f[6]);
	return 0;
}