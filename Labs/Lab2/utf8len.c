//#include<stdio.h>
//
//
//
//char Buffer[1000];
//
//int utf8len(char* c) {
//	for (int r = 0;;c++) {
//		if (r < 0) return -1;//To avoid endless String
//		if (!*c)//End of string
//			return r;
//		else if (*c > 0||//When *c is first byte of a UTF-8 code
//			(*c >= (char)0xC0 && *c <= (char)0xF7))
//			r++;
//	}
//}
//
//int main() {
//	for (;;) {
//		puts("Input a UTF-8 String.");
//		scanf("%s", Buffer);
//		printf("%d\n",utf8len(Buffer));
//	}
//
//	return 0;
//
//}
////严五洲NMSL