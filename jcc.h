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

bool equal(Token *tok, char *op);
Token *skip(Token *tok, char *op);
bool startswith(char *p, char *q);
Token *tokenize(char *p);

//
// parse.c
//

// AST node
typedef enum {
	ND_ADD, // +
	ND_SUB, // -
	ND_MUL, // *
	ND_DIV, // /
	ND_AND, // &
	ND_OR,  // |
	ND_LT,  // <
	ND_GT,  // >
	ND_EQ,  // = 代入でも比較演算子でも同じ '=' を使用するので注意
	ND_NEG, // unary -
	ND_NOT, // unary ~
	ND_RETURN, // 'return'
	ND_IF,     // 'if'
	ND_WHILE,  // 'while'
	ND_BLOCK,  // { ... }
	ND_NUM, // Integer
} NodeKind;

// AST node type
typedef struct Node Node;
struct Node {
	NodeKind kind; // Node kind
	Node *next;    // Next node
	Node *lhs;     // Left-hand side
	Node *rhs;     // Right-hand side

	// 'if' or 'while' statement
	Node *cond;
	Node *then;
	Node *els;

	// Block
	Node *body;

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
