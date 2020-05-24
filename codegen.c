#include "jcc.h"
static FILE *fout;

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
	default:
		fprintf(stderr, "不正な式です");
		return;
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

	gen_expr(node);

	fclose(fout);
}

