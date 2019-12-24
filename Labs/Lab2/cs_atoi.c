unsigned cs_atoi(char* c) {

	for (unsigned r = 0;; c++) {
		if (!*c)
			return r;
		else if (*c >= '0' && *c <= '9')
			r = (r << 3) + (r << 1) + *c - '0';
	}
}