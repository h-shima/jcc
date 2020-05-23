#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <errno.h>

extern char *cur_fname_without_ext;

//
// tokenize.c
//

// Token
typedef enum {
	TK_STR,      // String literals
	TK_RESERVED, // Keywords
	TK_SYMBOL,   // Single-letter punctuactors
	TK_IDENT,    // Identifier
	TK_NUM,      // Numeric literals
	TK_EOF,      // Enf-of-file markers
} TokenKind;

typedef struct Token Token;
struct Token {
	TokenKind kind; // Token kind
	Token *next;    // Next token
	long val;       // If kind is TK_NUM, its value
	char *loc;      // Token location
	int len;        // Token length

	char *contents; // String literal contents including terminating '  \0 '
	char cont_len;  // String literal length
};

Token *tokenize(char *p);

//
// parse.c
//

// AST node
typedef enum {
	ND_NUM, // Integer
} NodeKind;

// AST node type
typedef struct Node Node;
struct Node {
	NodeKind kind; // Node kind
	Node *next;    // Next node

	long val;      // Used if kind == ND_NUM
};

Node *parse(Token *tok);

//
// codegen.c
//
void codegen(Node *node);

//
// container.c
//
char *remove_extension(char *filename);
