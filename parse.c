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
// op = '+' | '-' | '*' | '/' | '&' | '|' | '<' | '>' | '='
// unaryOp = '-' | '~'
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

// expr = num (op num)*
// op = '+' | '-'
Node *parse(Token *tok) {
	Node head = {};
	Node *cur = &head;

	// １文字目は必ず数字
	Node *node = new_num(tok->val);
	tok = tok->next;

	for (;;) {
		if (equal(tok, "+")) {
			node = new_binary(ND_ADD, node, new_num(get_number(tok->next)));
			tok = tok->next->next;
			continue;
		}

		if (equal(tok, "-")) {
			node = new_binary(ND_SUB, node, new_num(get_number(tok->next)));
			tok = tok->next->next;
			continue;
		}

		if (equal(tok, "*")) {
			node = new_binary(ND_MUL, node, new_num(get_number(tok->next)));
			tok = tok->next->next;
			continue;
		}

		if (equal(tok, "/")) {
			node = new_binary(ND_DIV, node, new_num(get_number(tok->next)));
			tok = tok->next->next;
			continue;
		}

		cur = cur->next = node;
		return head.next;
	}
}
