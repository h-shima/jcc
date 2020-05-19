// https://qiita.com/nunu-e64/items/ff38888c88ec51c62921
#define STR(var) #var // 引数にした変数を変数名を示す文字列リテラルとして返すマクロ関数

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Token
typedef enum {
	TK_NUM, // Numeric literals
	TK_EOF, // Enf-of-file markers
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

		cur = cur->next;
	}

	return 0;
}
