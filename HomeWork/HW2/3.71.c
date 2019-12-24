#include <stdio.h>

#define BUFFER_SIZE 1024

void good_echo(){
    static char buffer[BUFFER_SIZE];
    int bufferptr;
	int flag = 1;
    while(flag && fgets(buffer, BUFFER_SIZE, stdin)) {
        printf("%s", buffer);
		for (bufferptr = 0; bufferptr < BUFFER_SIZE; bufferptr++) {
			if (buffer[bufferptr] == '\n') {
				flag = 0;
				break;
			}
		}
    }
    
    return;
}

int main()
{
    good_echo();

    return 0;
}