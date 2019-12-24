#include<stdbool.h>

bool isLittleEndian(){
    int test=0x01234567;
    char* charPtr = &test;
    return *charPtr==0x67;
}
