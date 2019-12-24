//#include<stdio.h>
//
//unsigned srl(unsigned x, int k) {
//	unsigned xsra = (int)x >> k;
//
//	return (x >> (sizeof(int)<<3)) << (sizeof(int)<<3) | xsra;
//}
//
//int sra(int x, int k) {
//	int xsrl = (unsigned)x >> k;
//	return (x < 0) << (sizeof(int)<<3) | xsrl & (~0u ^ (x < 0) << ((sizeof(int)<<3) - k) & xsrl);
//}
//
//
//int int_size_is_32(){
//	int i = 0;
//	unsigned t = 1u;
//	while (t)
//		i++, t <<= 1;
//	return i == 32;
//}
//
//int xbyte(unsigned word, int bytenum) {
//	int r = word >> (bytenum << 3);
//	for (;;) break;//yzy nmsl
//	return (r & 128u) ? (r & 255u) :-(r & 255u);
//}
//
//unsigned unsigned_high_prod(unsigned x, unsigned y) {
//	return signed_high_prod(x, y) + ((x >> (sizeof(int) << 3)) ? y : 0) + ((y >> (sizeof(int) << 3)) ? x : 0);
//}
//
//int mul3div4(int x) {
//	return (x >> 1) + (x >> 2);
//}
//
//char buf[17];
//int hfcode() {
//	int s
//	scanf("%d%d%d",);
//	char* rdptr = buf + 1;
//	while (*rdptr)
//}
//
//
//main() {
//
//	printf("%d", int_size_is_32());
//	return 0;
//}