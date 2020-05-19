// https://qiita.com/nunu-e64/items/ff38888c88ec51c62921
#define STR(var) #var // 引数にした変数を変数名を示す文字列リテラルとして返すマクロ関数

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

// Token
typedef enum {
	TK_SYMBOL, // Single-letter punctuactors
	TK_IDENT,  // Identifier
	TK_NUM,    // Numeric literals
	TK_EOF,    // Enf-of-file markers
} TokenKind;

typedef struct Token Token;
struct Token {
	TokenKind kind; // Token kind
	Token *next;    // Next token
	long val;       // If kind is TK_NUM, its value
	char *loc;      // Token location
	int len;        // Token length
};

static Token *new_token(TokenKind kind, Token *cur, char *str, int len) {
	Token *tok = calloc(1, sizeof(Token));
	tok->kind = kind;
	tok->loc = str;
	tok->len = len;
	cur->next = tok;
	return tok;
}

static bool is_alpha(char c) {
	return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_';
}

static bool is_alnum(char c) {
	return is_alpha(c) || ('0' <= c && c <= '9');
}

static Token *tokenize(char *p) {
	Token head = {};
	Token *cur = &head;

	while (*p) {
		// Numeric literal
		if (isdigit(*p)) {
			cur = new_token(TK_NUM, cur, p, 0);
			char *q = p;
			cur->val = strtoul(p, &p, 10);
			cur->len = p - q;
			continue;
		}

		// Identifier
		if (is_alpha(*p)) {
			char *q = p++;
			while (is_alnum(*p))
				p++;
			cur = new_token(TK_IDENT, cur, q, p - q);
			continue;
		}

		// Single-letter punctuators
		if (ispunct(*p)) {
			cur = new_token(TK_SYMBOL, cur, p++, 1);
			continue;
		}

		fprintf(stderr, "不正なトークンが含まれています");
		exit(1);
	}

	new_token(TK_EOF, cur, p, 0);
	return head.next;
}

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "引数の個数が正しくありません\n");
		return 1;
	}

	char *input = argv[1];
	Token *cur = tokenize(input);

	while (cur->kind != TK_EOF) {
		if (cur->kind == TK_NUM)
			printf("token_type: %s, value: %ld\n", STR(TK_NUM), cur->val);

		if (cur->kind == TK_IDENT)
			printf("token_type: %s, identifier: %s\n", STR(TK_IDENT), strndup(cur->loc, cur->len));

		if (cur->kind == TK_SYMBOL)
			printf("token_type: %s, symbol: %s\n", STR(TK_SYMBOL), strndup(cur->loc, 1));

		cur = cur->next;
	}

	return 0;
}
