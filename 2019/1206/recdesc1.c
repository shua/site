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

Pout Pexpr(Pin);

Pout
Ppar(Pin in) {
	Pout out, ret={0};
	if (out = Pchar(in, '('), out.parsed == -1)
		return PARSE_FAIL;
	ret = out;
	in.buf += out.parsed;
	if (out = Pexpr(in), out.parsed == -1)
		return PARSE_FAIL;
	ret = (Pout){
		.parsed = ret.parsed + out.parsed,
		.pres = out.pres
	};
	in.buf += out.parsed;
	if (out = Pchar(in, ')'), out.parsed == -1)
		return PARSE_FAIL;
	ret.parsed += out.parsed;
	return ret;
}

Pout
Pnot(Pin in) {
	Pout out, ret = {0};
	if (out = Pchar(in, '!'), out.parsed == -1)
		return PARSE_FAIL;
	ret = out;
	in.buf += out.parsed;
	if (out = Pexpr(in), out.parsed == -1)
		return PARSE_FAIL;
	ret = (Pout){
		.parsed = ret.parsed + out.parsed,
		.pres = !out.pres
	};
	return ret;
}

Pout
Pexpr(Pin in) {
	Pout out;
	if (out = Ptrue(in), out.parsed != -1)
		return out;
	if (out = Pfalse(in), out.parsed != -1)
		return out;
	if (out = Ppar(in), out.parsed != -1)
		return out;
	if (out = Pnot(in), out.parsed != -1)
		return out;
	return PARSE_FAIL;
}

int
main(void) {
	Pin in[] ={
		{ .buf = "t" },
		{ .buf = "f" },
		{ .buf = "e" },
		{ .buf = "!t" },
		{ .buf = "(t)" },
		{ .buf = "!(!f)" },
		{ .buf = "(" },
		{ .buf = ")" },
		{ .buf = "!" }
	};
	Pout out;

	for (int i=0; i < sizeof(in)/sizeof(in[0]); i++) {
		out = Pexpr(in[i]);
		printf("%s = %d [%d]\n", in[i].buf, out.pres, out.parsed);
	}

	return 0;
}
