#include "jcc.h"
static FILE *fout;

static void gen_expr(Node *node) {
	if (node->kind == ND_NUM) {
		fprintf(fout, "push constant %ld\n", node->val);
		return;
	}

	gen_expr(node->lhs);
	gen_expr(node->rhs);

	if (node->kind == ND_ADD) {
		fprintf(fout, "add\n");
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

