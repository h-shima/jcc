#include "jcc.h"

void codegen(Node *node) {
	FILE *fout;

	char *fout_filename = cur_fname_without_ext;
	strcat(fout_filename, ".vm");

	fout = fopen(fout_filename, "w");
	if (fout == NULL) {
		fprintf(stderr, "出力ファイルをオープンできません");
		exit(1);
	}

	for (Node *cur = node; cur; cur = cur->next) {
		fprintf(fout, "%ld\n", cur->val);
	}

	fclose(fout);
}
