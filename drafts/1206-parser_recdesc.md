<pmeta id="created">2019 December 06</pmeta>
<pmeta id="title">Parsing 2: recursive descent</pmeta>

[Last time](./1109-parser1_PDA.html)
I tried to parse our toy logical calculator language using a hand-constructed push-down automata.

Today, I'll introduce another method of writing parsers: recursive descent.
In contrast to PDAs, recursive descent parsers read a lot like the original grammar, and so require a little less mental translation to write.

However, the problem of infinite recursion is introduced, and it can be especially tricky to avoid infinite recursion in recursive descent parsers for grammars that are recursive. 

The basic crux of a recursive descent parser is that you define complex parsing functions in terms of simpler parsing functions.
Often the functions you define can be mapped 1-1 to non-terminal expressions in your grammar.

Starting from The Bottom
---

We need some simple starting parsers to start with that don't reference any others, so let's define `Pchar`

	Pout
	Pchar(Pin in, char c) {
		if (in.buf[0] == c)
			return (Pout){ .parsed=1 };
		return PARSE_FAIL;
	}

In this example I make use of some types (`Pout`, `Pin`, and a constant called `PARSE_FAIL`), which can be defined as follows

	typedef struct {
		int parsed;
		bool pres;
	} Pout;
	typedef struct {
		char* buf;
	} Pin;
	const Pout PARSE_FAIL = (Pout){ .parsed=-1 };

The `Pout` struct contains the number of chars parsed (or -1 if parsing failed), as well as any information that needs to be threaded back to the caller.
Since we are interested only in the evaluated logical string, we only ever need to pass back a boolean value (`bool pres`).
If you're building a parser for the purpose of, say, pretty-printing the string, you'd want to return something with a little more information (like an Abstract Syntax Tree or "AST").

As a reminder, our grammar is defined as

	expr ::=
		| 't'
		| 'f'
		| '(' expr ')'
		| '!' expr
		| expr '&' expr
		| expr '|' expr
		;

So, let's try parsing something simple, like `'t'`.

	Pout
	Ptrue(Pin in) {
		Pout out;
		if (out = Pchar(in, 't'), out.parsed == -1)
			return PARSE_FAIL;
		out.pres = true;
		return out;
	}

and I guess a similar one for parsing false values

	Pout
	Pfalse(Pin in) {
		Pout out;
		if (out = Pchar(in, 't'), out.parsed == -1)
			return PARSE_FAIL;
		out.pres = true;
		return out;
	}

It's certainly a little more verbose than our grammar, but it sort of makes sense, and let's us parse the simpler grammar of

	expr ::= 't' | 'f'

let's implement the `expr`

	Pout
	Pexpr(Pin in) {
		Pout out;
		if (out = Ptrue(in), out.parsed != -1)
			return out;
		if (out = Pfalse(in), out.parsed != -1)
			return out;
		return PARSE_FAIL;
	}

and a `main` that tests it

	int
	main(int argc, char\** argv) {
		Pin in[] ={
			{ .buf = "t" },
		   { .buf = "f" },
		   { .buf = "e" }
		};
		Pout out;

		for (int i=0; i < sizeof(in)/sizeof(in[0]); i++) {
			out = Pexpr_tf(in[i]);
			fprintf("%s = %d [%d]\n", in[i].buf, out.pres, out.parsed);
		}

		return 0;
	}


The Descent
---

Now we can move to parens, and not, which are both defined recursively in terms of `expr`.

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


Let's add not, which needs to parse an expression and return the logical not of it

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

and last add those cases to `Pexpr`, and some extra cases to see that it works

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
	
	main ...
		Pin in[] = {
			...
			{ .buf = "!t" },
			{ .buf = "(t)" },
			{ .buf = "!(!f)" },
			{ .buf = "(" },
			{ .buf = ")" },
			{ .buf = "!" }
		};

Binops in the Deep
---

We are speeding right along, so `and` and `or` are next

	Pout
	Pand(Pin in) {
		Pout out, ret = {0};
		if (out = Pexpr(in), out.parsed == -1)
			return PARSE_FAIL;
		ret = out;
		in.buf += out.parsed;
		if (out = Pchar(in, '&') == -1)
			return PARSE_FAIL;
		ret.parsed += out.parsed;
		in.buf += out.parsed;
		if (out = Pexpr(in), out.parsed == -1)
			return PARSE_FAIL;
		ret = (Pout){
			.parsed = ret.parsed + out.parsed,
			.pres = ret.pres && out.pres
		};
		return ret;
	}
	
	Pout
	Por(Pin in) {
		Pout out, ret = {0};
		if (out = Pexpr(in), out.parsed == -1)
			return PARSE_FAIL;
		ret = out;
		in.buf += out.parsed;
		if (out = Pchar(in, '|') == -1)
			return PARSE_FAIL;
		ret.parsed += out.parsed;
		in.buf += out.parsed;
		if (out = Pexpr(in), out.parsed == -1)
			return PARSE_FAIL;
		ret = (Pout){
			.parsed = ret.parsed + out.parsed,
			.pres = ret.pres || out.pres
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
		if (out = Pand(in), out.parsed != -1)
			return out;
		if (out = Por(in), out.parsed != -1)
			return out;
		return PARSE_FAIL;
	}
	
	main ...
		Pin in[] = {
			...
			{ .buf = "t&f" },
			{ .buf = "t|f" }
		};
		...
	

now, wait a second, those last test cases result in

	t&f = t [1]
	t|f = t [1]

that's not right, I want it to parse the whole string, but instead, it's short-circuiting at the first `t`.
We could put the `and` and `or` cases before the others, so it tries those first.


	Pout
	Pexpr(Pin in) {
		Pout out;
		if (out = Pand(in), out.parsed != -1)
			return out;
		if (out = Por(in), out.parsed != -1)
			return out;
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

Perfect, now just start 'er up and...

	Segmentation fault

Well that's no good.

Left Be Recursed
---



**-JD**
