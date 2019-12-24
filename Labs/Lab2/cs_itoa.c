void cs_itoa(unsigned i, char* t) {
	const char* stk = malloc(16 * sizeof(char));
	char* stkptr = stk;
	for (; i; i /= 10)
		*++stkptr = i % 10;
	for (; stkptr != stk; t++, stkptr--)
		*t = *stkptr + '0';
	free(stk);
}