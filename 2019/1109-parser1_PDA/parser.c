#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef enum State { S_S, S_E } State;
typedef enum Op { Nop, Not, And, Or } Op;
typedef struct OpList {
	Op op;
	bool l;
	struct OpList* next;
} OpList;

void
printstack(OpList* os) {
	printf("OPSTACK: ");
	while (os) {
		switch (os->op) {
		case Nop: printf("Nop, "); break;
		case Not: printf("Not, "); break;
		case Or: printf("Or(%c), ", os->l?'t':'f'); break;
		case And: printf("And(%c), ", os->l?'t':'f'); break;
		}
		os = os->next;
	}
	printf("\n");
}

OpList*
push(OpList* os, Op op, bool l) {
	OpList* r = malloc(sizeof(OpList));
	*r = (OpList){ .op=op, .l=l, .next=os };
	// printf("PUSH "); printstack(r);
	return r;
}

OpList
pop(OpList* os) {
	// printf("POP "); printstack(os);
	OpList r = *os;
	free(os);
	return r;
}

OpList*
eval(OpList* os, bool* b) {
	switch (os->op) {
	case Nop: return os;
	case Not: *b=!*b; return os = pop(os).next;
	case Or: *b = os->l || *b; return os = pop(os).next;
	case And: *b = os->l && *b; return os = pop(os).next;
	}
}

int
parse(bool* b) {
	int c;
	State s = S_S;
	OpList* os = push(NULL, Nop, *b);

#define err(FMT, ...) return (fprintf(stderr, "ERROR: %c,'%c': " FMT "\n", \
		s==S_S?'S':'E', c=='\n'?'\\':c, \
		##__VA_ARGS__), printstack(os), 2)

	while ((c=getchar()) != EOF && c != '\n')
	switch (s) {
	case S_S: switch (c) {
		case 't': *b = true; os = eval(os, b); s = S_E; break;
		case 'f': *b = false; os = eval(os, b); s = S_E; break;
		case '!': os = push(os, Not, *b); break;
		case '(': os = push(os, Nop, *b); break;
		default: err("Unexpected");
		}
		break;

	case S_E: switch (c) {
		case '&': os = push(os, And, *b); s = S_S; break;
		case '|': os = push(os, Or, *b); s = S_S; break;
		case ')': ;
			if (!os->next || os->op != Nop) err("Unexpected");
			os = pop(os).next;
			os = eval(os, b);
			break;
		default: err("Unexpected");
		}
		break;
	}

	if (os && os->next) err("Expected more");

	pop(os);
#undef err
	return s != S_E;
}

int
main(void) {
	bool ret;
	if (parse(&ret) != 0) {
		fprintf(stderr, "failed to parse\n");
		return 1;
	}
	printf("%c\n", ret ? 't' : 'f');
	return 0;
}
