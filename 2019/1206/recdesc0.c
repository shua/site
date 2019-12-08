#include<stdio.h>
#include<stdbool.h>

typedef struct {
	int parsed;
	bool pres;
} Pout;
typedef struct {
	char* buf;
} Pin;
const Pout PARSE_FAIL = (Pout){ .parsed=-1 };


Pout
Pchar(Pin in, char c) {
	if (in.buf[0] == c)
		return (Pout){ .parsed=1 };
	return PARSE_FAIL;
}

Pout
Ptrue(Pin in) {
	Pout out;
	if (out = Pchar(in, 't'), out.parsed == -1)
		return PARSE_FAIL;
	out.pres = true;
	return out;
}

Pout
Pfalse(Pin in) {
	Pout out;
	if (out = Pchar(in, 'f'), out.parsed == -1)
		return PARSE_FAIL;
	out.pres = false;
	return out;
}

Pout
Pexpr(Pin in) {
	Pout out;
	if (out = Ptrue(in), out.parsed != -1)
		return out;
	if (out = Pfalse(in), out.parsed != -1)
		return out;
	return PARSE_FAIL;
}

int
main(void) {
	Pin in[] ={
		{ .buf = "t" },
	   { .buf = "f" },
	   { .buf = "e" }
	};
	Pout out;

	for (int i=0; i < sizeof(in)/sizeof(in[0]); i++) {
		out = Pexpr(in[i]);
		printf("%s = %d [%d]\n", in[i].buf, out.pres, out.parsed);
	}

	return 0;
}
