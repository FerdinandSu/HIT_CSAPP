#include<stdio.h>

char Char;
short Short;
int Int;
long long Long;

float Float;
double Double;
long double LongDouble;

unsigned char Byte;
unsigned short UShort;
unsigned int UInt;
unsigned long long ULong;


char* Pointer, Address_main, Address_printf;
int Array[1];
struct MyStruct
{
	int a;
} Struct;

union MyUnion
{
	int a;
	char c;
} Union;

enum MyEnum
{
	v1=1,v2=2
} Enum;

typedef enum MyEnum Enum_t;

static inline void print_hex_mem(char* target, unsigned int size) {
	while (size--)
		printf("%x ", *(target++));
	putchar('\n');
}

int main() {
	Char = Byte = '0';
	Short = 1, UShort = 1u;
	Int = 2, UInt = 2u;
	Long = 3ll, ULong = 3ull;
	Float = 4.0f, Double = 5.0, LongDouble = 6.0l;
	Pointer = &Byte;
	Address_main = main;
	Address_printf = printf;
	Array[1] = 7;
	Struct.a = 8;
	Union.a = 9;
	Enum = v1;

	printf("Var. Name\t\tContent\t\t\tAddress\t\t\t\t\tHex. Memory\n");
	printf("Char\t\t\t%c\t\t\t%p\t\t\t", Char, &Char);
	print_hex_mem(&Char, sizeof(Char));
	printf("Short\t\t\t%d\t\t\t%p\t\t\t", Short, &Short);
	print_hex_mem(&Short, sizeof(Short));
	printf("Int\t\t\t%d\t\t\t%p\t\t\t", Int, &Int);
	print_hex_mem(&Int, sizeof(Int));
	printf("Long\t\t\t%lld\t\t\t%p\t\t\t", Long, &Long);
	print_hex_mem(&Long, sizeof(Long));
	printf("Byte\t\t\t%u\t\t\t%p\t\t\t", Byte, &Byte);
	print_hex_mem(&Byte, sizeof(Byte));
	printf("UShort\t\t\t%u\t\t\t%p\t\t\t", UShort, &UShort);
	print_hex_mem(&UShort, sizeof(UShort));
	printf("UInt\t\t\t%u\t\t\t%p\t\t\t", UInt, &UInt);
	print_hex_mem(&UInt, sizeof(UInt));
	printf("ULong\t\t\t%llu\t\t\t%p\t\t\t", ULong, &ULong);
	print_hex_mem(&ULong, sizeof(ULong));
	printf("Short\t\t\t%d\t\t\t%p\t\t\t", Short, &Short);
	print_hex_mem(&Short, sizeof(Short));
	printf("Float\t\t\t%f\t\t%p\t\t\t", Float, &Float);
	print_hex_mem(&Float, sizeof(Float));
	printf("Double\t\t\t%lf\t\t%p\t\t\t", Double, &Double);
	print_hex_mem(&Double, sizeof(Double));
	printf("LongDouble\t\t%llf\t\t%p\t\t\t", LongDouble, &LongDouble);
	print_hex_mem(&LongDouble, sizeof(LongDouble));
	printf("Pointer\t\t\t%p\t%p\t\t\t", Pointer, &Pointer);
	print_hex_mem(&Pointer, sizeof(Pointer));
	printf("Address_main\t\t%p\t%p\t\t\t", Address_main, &Address_main);
	print_hex_mem(&Address_main, sizeof(Address_main));
	printf("Address_printf\t\t%p\t%p\t\t\t", Address_printf, &Address_printf);
	print_hex_mem(&Address_printf, sizeof(Address_printf));
	printf("Array\t\t\t{%x}\t\t\t%p\t\t\t", Array[0], &Array);
	print_hex_mem(&Array, sizeof(Array));
	printf("Struct\t\t\t{%x}\t\t\t%p\t\t\t", Struct, &Struct);
	print_hex_mem(&Struct, sizeof(Struct));
	printf("Union\t\t\t{%x}\t\t\t%p\t\t\t", Union, &Union);
	print_hex_mem(&Union, sizeof(Union));
	printf("Enum\t\t\t%d\t\t\t%p\t\t\t", Enum, &Enum);
	print_hex_mem(&Enum, sizeof(Enum));
	return 0;
}