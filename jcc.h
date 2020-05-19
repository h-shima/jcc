// https://qiita.com/nunu-e64/items/ff38888c88ec51c62921
#define STR(var) #var // 引数にした変数を変数名を示す文字列リテラルとして返すマクロ関数

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <errno.h>

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