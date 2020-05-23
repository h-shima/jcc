#include "jcc.h"

void codegen(Node *node) {
	FILE *fout;
	fout = fopen("Main.vm", "w");
	if (fout == NULL) {
		fprintf(stderr, "出力ファイルをオープンできません");
		exit(1);
	}

	for (Node *cur = node; cur; cur = cur->next) {
		fprintf(fout, "%ld\n", cur->val);
	}

	fclose(fout);
}
