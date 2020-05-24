#include "jcc.h"

// Consumes the current token if it matches `op`
bool equal(Token *tok, char *op) {
	return strlen(op) == tok->len &&
		!strncmp(tok->loc, op, tok->len);
}

static bool startswith(char *p, char *q) {
	return strncmp(p, q, strlen(q)) == 0;
}

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

static bool is_keyword(Token *tok) {
	static char *kw[] = {"class", "constructor", "function", "method", "field", "static", "var", "int", "char", "boolean",
					  "void", "true", "false", "null", "this", "let", "do", "if", "else", "while", "return"};

	for (int i = 0; i < sizeof(kw) / sizeof(*kw); i++)
		if (equal(tok, kw[i]))
			return true;
	return false;
}

static void convert_keywords(Token *tok) {
	for (Token *t = tok; t->kind != TK_EOF; t = t->next)
		if (t->kind == TK_IDENT && is_keyword(t))
			t->kind = TK_RESERVED;
}

static Token *read_string_literal(Token *cur, char *start) {
	char *p = start + 1;
	char *end = p;

	// Find the closing double-quote.
	for (; *end != '"'; end++)
		if (*end == '\0') {
			fprintf(stderr, "ダブルクォートの終端が閉じられていません\n");
			exit(1);
		}

	// Allocate a buffer that is large enough to hold the entire string literal.
	char *buf = malloc(end - p + 1);
	int len = 0;

	while (*p != '"') {
		buf[len++] = *p++;
	}
	buf[len++] = '\0';

	Token *tok = new_token(TK_STR, cur, start, p - start + 1);
	tok->contents = buf;
	tok->cont_len = len;
	return tok;
}

Token *tokenize(char *p) {
	Token head = {};
	Token *cur = &head;

	while (*p) {
		// Skip line comments.
		if (startswith(p, "//")) {
			p += 2;
			while (*p != '\n')
				p++;
			continue;
		}

		// Skip block comments.
		if (startswith(p, "/*")) {
			char *q = strstr(p +2, "*/");
			if (!q)
				fprintf(stderr, "ブロックコメントが閉じられていません");
			p = q + 2;
			continue;
		}

		// Skip whitespace characters.
		if (isspace(*p)) {
			p++;
			continue;
		}

		// Numeric literal
		if (isdigit(*p)) {
			cur = new_token(TK_NUM, cur, p, 0);
			char *q = p;
			cur->val = strtoul(p, &p, 10);
			cur->len = p - q;
			continue;
		}

		// String literal
		if (*p == '"') {
			cur = read_string_literal(cur, p);
			p += cur->len;
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
		fprintf(stderr, "%c", *p);
		exit(1);
	}

	new_token(TK_EOF, cur, p, 0);
	convert_keywords(head.next);
	return head.next;
}
