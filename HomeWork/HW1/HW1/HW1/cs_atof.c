double cs_atof(char* c) {
	double r = 0, m = 1e1;
	int flag = 0, is_minus = 0;
	for (;; c++) {
		if (!*c)
			return is_minus ? -r : r;
		else if (*c == '.')
			flag = 1;
		else if (*c == '-')
			is_minus = 1;
		else if (*c >= '0' && *c <= '9') {
			if (flag)
				r += (*c - '0') / m, m *= 10;
			else
				r = r * 10 + *c - '0';
		}
	}
}