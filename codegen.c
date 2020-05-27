#include "jcc.h"
static FILE *fout;
static int labelseq = 1;

static void gen_expr(Node *node) {
	if (node->kind == ND_NUM) {
		fprintf(fout, "push constant %ld\n", node->val);
		return;
	}

	if (node->kind == ND_NEG) {
		gen_expr(node->lhs);
		fprintf(fout, "neg\n");
		return;
	}

	if (node->kind == ND_NOT) {
		gen_expr(node->lhs);
		fprintf(fout, "not\n");
		return;
	}

	gen_expr(node->lhs);
	gen_expr(node->rhs);

	switch (node->kind) {
    case ND_ADD:
        fprintf(fout, "add\n");
        return;
	case ND_SUB:
		fprintf(fout, "sub\n");
		return;
	case ND_MUL:
		fprintf(fout, "call Math.multiply 2\n");
		return;
	case ND_DIV:
		fprintf(fout, "call Math.divide 2\n");
		return;
	case ND_AND:
		fprintf(fout, "and\n");
		return;
	case ND_OR:
		fprintf(fout, "or\n");
		return;
	case ND_LT:
		fprintf(fout, "lt\n");
		return;
	case ND_GT:
		fprintf(fout, "gt\n");
		return;
	case ND_EQ:
		fprintf(fout, "eq\n");
		return;
	default:
		fprintf(stderr, "不正な式です");
		return;
	}
}

static void gen_stmt(Node *node) {
	switch (node->kind) {
	case ND_RETURN:
		gen_expr(node->lhs);
		fprintf(fout, "return\n");
		return;
	case ND_BLOCK:
		for (Node *n = node->body; n; n = n->next)
			gen_stmt(n);
		return;
	case ND_IF:{
		int seq = labelseq++;

		gen_expr(node->cond);
		fprintf(fout, "if-goto IF_TRUE%d\n", seq);
		fprintf(fout, "goto IF_FALSE%d\n", seq);
		fprintf(fout, "label IF_TRUE%d\n", seq);
		gen_stmt(node->then);
		fprintf(fout, "goto IF_END%d\n", seq);
		fprintf(fout, "label IF_FALSE%d\n", seq);
		if (node->els != NULL)
			gen_stmt(node->els);
		fprintf(fout, "label IF_END%d\n", seq);
		return;
	}
	default:
		fprintf(stderr, "不正な文です。");
	}
}

void codegen(Node *node) {
	char *fout_filename = cur_fname_without_ext;
	strcat(fout_filename, ".vm");

	fout = fopen(fout_filename, "w");
	if (fout == NULL) {
		fprintf(stderr, "出力ファイルをオープンできません");
		exit(1);
	}

	gen_stmt(node);

	fclose(fout);
}

