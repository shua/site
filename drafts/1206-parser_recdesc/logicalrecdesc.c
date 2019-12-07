#import<stdio.h>
#import<stdbool.h>
#import<stdlib.h>

int debug_level = 0;
void
print_indent(int i) {
	while (i-- > 0)
		fprintf(stderr, "  ");
}
void _debug() {}
#define debug(F, ...) ((debug_level > 0) ? (print_indent(in.depth), fprintf(stderr, F "\n", ##__VA_ARGS__)) : _debug())

typedef struct {
	int parsed;
	bool pres;
} Pout;
const Pout PARSE_FAIL = (Pout){ .parsed=-1 };

typedef struct {
	char *buf;
	int depth;
} Pin;


Pout
Pchar(Pin in, char c) {
	in.depth++;
	debug("Pchar '%c'", c);
	if (in.buf[0] == c)
		return (Pout){ .parsed=1 };
	debug("fail");
	return PARSE_FAIL;
}

Pout Pexpr(Pin in);

Pout
Ppar(Pin in) {
	Pout out, res={0};
	in.depth++;
	debug("Ppar");
	if (out=Pchar(in, '('), out.parsed == -1)
		return PARSE_FAIL;
	res.parsed += out.parsed;
	in.buf += out.parsed;
	if (out=Pexpr(in), out.parsed == -1)
		return PARSE_FAIL;
	res = (Pout){ .parsed=res.parsed + out.parsed, .pres=out.pres };
	in.buf += out.parsed;
	if (out=Pchar(in, ')'), out.parsed == -1)
		return PARSE_FAIL;
	res.parsed += out.parsed;
	debug("success");
	return res;
}

Pout
Pval(Pin in) {
	Pout out;
	debug("Pval");
	in.depth++;
	if (out=Ppar(in), out.parsed != -1)
		return out;
	if (out=Pchar(in, 't'), out.parsed != -1)
		return (Pout){ .parsed=1, .pres=1 };
	if (out=Pchar(in, 'f'), out.parsed != -1)
		return (Pout){ .parsed=1, .pres=0 };
	debug("fail");
	return PARSE_FAIL;
}

Pout
Por(Pin in) {
	Pout out, res={0};
	debug("Por");
	in.depth++;
	if (out=Pval(in), out.parsed == -1)
		return PARSE_FAIL;
	res = out;
	in.buf += out.parsed;
	if (out=Pchar(in, '|'), out.parsed == -1)
		return PARSE_FAIL;
	res.parsed += out.parsed;
	in.buf += out.parsed;
	if (out=Pexpr(in), out.parsed == -1)
		return PARSE_FAIL;
	res = (Pout){ .parsed=res.parsed + out.parsed, .pres=res.pres || out.pres };
	debug("success");
	return res;
}

Pout
Pand(Pin in) {
	Pout out, res={0};
	debug("Pand");
	in.depth++;
	if (out=Pval(in), out.parsed == -1)
		return PARSE_FAIL;
	res = out;
	in.buf += out.parsed;
	if (out=Pchar(in, '&'), out.parsed == -1)
		return PARSE_FAIL;
	res.parsed += out.parsed;
	in.buf += out.parsed;
	if (out=Pexpr(in), out.parsed == -1)
		return PARSE_FAIL;
	res = (Pout){ .parsed=res.parsed + out.parsed, .pres=res.pres && out.pres };
	debug("success");
	return res;
}

Pout
Pexpr(Pin in) {
	Pout out;
	debug("Pexpr");
	in.depth++;
	if (out=Por(in), out.parsed != -1)
		return out;
	if (out=Pand(in), out.parsed != -1)
		return out;
	if (out=Pval(in), out.parsed != -1)
		return out;
	debug("fail");
	return PARSE_FAIL;
}

int
main(int argc, char** argv) {
	int c;
	char* buf;
	int bufz = 32;
	int bufi = 0;
	int ret = 0;
	Pout out;

	if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'g')
		debug_level++;

	buf = malloc(bufz);
	while ((c=getc(stdin)) != EOF && c != '\n') {
		while (bufi+1 >= bufz)
			buf = realloc(buf, bufz*=2);
		if (!buf)
			return (fprintf(stderr, "mem error\n"), 2);

		buf[bufi++] = c;
	}
	buf[bufi] = 0;

	if ((out=Pexpr((Pin){ .buf=buf, .depth=0 })).parsed != bufi) {
		printf("Parse failure\n");
		ret = 1;
	} else {
		printf("%s = %c\n", buf, out.pres?'t':'f');
	}

	free(buf);
	return ret;
}

