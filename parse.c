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

Node *parse(Token *tok) {
	Node head = {};
	Node *cur = &head;

	while (tok->kind != TK_EOF) {
		Node *node = calloc(1, sizeof(Node));
		node->kind = ND_NUM;
		node->val = tok->val;
		cur = cur->next = node;

		tok = tok->next;
	}

	return head.next;
}
