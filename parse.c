#include "jcc.h"

// 未実装のパース関数
// program = class
// class = 'class' identifier '{' classVarDec* subroutineDec* '}'
// classVarDec = ('static' | 'field') type varName (',' varName)* ';'
// type = 'int' | 'char' | 'boolean' | identifier
// subroutineDec = ('constructor' | 'function' | 'method') ('void' | type) subroutineName '(' parameterList ')' subroutineBody
// parameterList = ((type varName) (',' type varName)*)?
// subroutineBody = '{' varDec* statements '}'
// varDec = 'var' type varName (',' varName)* ';'

// statements = statement*
// statement = letStatement
//           | ifStatement
//           | whileStatement
//           | doStatement
//           | returnStatement
// letStatement = 'let' varName ('[' expression ']')? '=' expression ';'
// ifStatement = 'if' '(' expression ')' '{' statements '}' ('else' '{' statements '}')?
// whileStatement = 'while' '(' expression ')' '{' statements '}'
// doStatement = 'do' subroutineCall ';'
// returnStatement = 'return' expression? ';'

// expression = term (op term)*
// term = integerConstant
//      | stringConstant
//      | keywordConstant
//      | varName ('[' expression ']')?
//      | subroutineCall
//      | '(' expression ')'
//      | unaryOp term
// subroutineCall = subroutineName '(' expressionList ')
//                | identifier '.' identifier '(' expressionList ')'
// expressionList = (expression (',' expression)*)?
// keywordConstant = 'true' | 'false' | 'null' | 'this'
static Node *new_node(NodeKind kind) {
	Node *node = calloc(1, sizeof(Node));
	node->kind = kind;
	return node;
}

static Node *new_binary(NodeKind kind, Node *lhs, Node *rhs) {
	Node *node = new_node(kind);
	node->lhs = lhs;
	node->rhs = rhs;
	return node;
}

static Node *new_num(long val) {
	Node *node = new_node(ND_NUM);
	node->val = val;
	return node;
}

static long get_number(Token *tok) {
	if (tok->kind != TK_NUM)
		fprintf(stderr, "数値ではありません");
	return tok->val;
}

// term = integerConstant
//      | unary_op term
// unary_op = '-' | '~'
Node *term(Token **rest, Token *tok) {
	if (startswith(tok->loc, "-")) {
		Node *node = new_node(ND_NEG);
		node->lhs = term(rest, tok->next);
		return node;
	}

	if (startswith(tok->loc, "~")) {
		Node *node = new_node(ND_NOT);
		node->lhs = term(rest, tok->next);
		return node;
	}

	Node *node = new_num(tok->val);

	*rest = tok->next;
	return node;
}

// expr = term (op term)*
// term = integerConstant
//      | unary_op term
// unaryOp = '-' | '~'
// op = '+' | '-' | '*' | '/' | '&' | '|' | '<' | '>' | '='
static Node *expr(Token **rest, Token *tok) {
	Node *node = term(&tok, tok);

	if (tok->kind != TK_EOF) {
		for (;;) {
			if (equal(tok, "+")) {
				node = new_binary(ND_ADD, node, term(&tok, tok->next));
				continue;
			}

			if (equal(tok, "-")) {
				node = new_binary(ND_SUB, node, term(&tok, tok->next));
				continue;
			}

			if (equal(tok, "*")) {
				node = new_binary(ND_MUL, node, term(&tok, tok->next));
				continue;
			}

			if (equal(tok, "/")) {
				node = new_binary(ND_DIV, node, term(&tok, tok->next));
				continue;
			}

			if (equal(tok, "&")) {
				node = new_binary(ND_AND, node, term(&tok, tok->next));
				continue;
			}

			if (equal(tok, "|")) {
				node = new_binary(ND_OR, node, term(&tok, tok->next));
				continue;
			}

			if (equal(tok, "<")) {
				node = new_binary(ND_LT, node, term(&tok, tok->next));
				continue;
			}

			if (equal(tok, ">")) {
				node = new_binary(ND_GT, node, term(&tok, tok->next));
				continue;
			}

			if (equal(tok, "=")) {
				node = new_binary(ND_EQ, node, term(&tok, tok->next));
				continue;
			}

			*rest = tok;
			return node;
		}
	}
	return node;
}

// statement = returnStatement
// returnStatement = 'return' expression? ';'
static Node *stmt(Token **rest, Token *tok) {
	if (equal(tok, "return")) {
		Node *node = new_node(ND_RETURN);

		if (!equal(tok->next, ";"))
			node->lhs = expr(&tok, tok->next);

		*rest = skip(tok, ";");
		return node;
	}

	fprintf(stderr, "不正な文です。");
	exit(1);
}

// program = stmt
Node *parse(Token *tok) {
	Node *node;
	node = stmt(&tok, tok);
	return node;
}

