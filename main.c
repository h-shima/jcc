#include "jcc.h"

// 指定されたファイルの内容を返す
char *read_file(char *path) {
	// ファイルを開く
	FILE *fp = fopen(path, "r");
	if (!fp)
		fprintf(stderr, "%sファイルがオープンできません", path);

	// ファイルの長さを調べる
	if (fseek(fp, 0, SEEK_END) == -1)
		fprintf(stderr, "%sファイルのfseekでエラーが発生しました", path);
	size_t size = ftell(fp);
	if (fseek(fp, 0, SEEK_SET) == -1)
		fprintf(stderr, "%sファイルのfseekでエラーが発生しました", path);

	// ファイル内容を読み込む
	char *buf = calloc(1, size + 2);
	fread(buf, size, 1, fp);

	// ファイルが必ず"\n\0"で終わるようにする
	if (size == 0 || buf[size - 1] != '\n')
		buf[size++] = '\n';
	buf[size] = '\0';
	fclose(fp);
	return buf;
}


int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "引数の個数が正しくありません\n");
		return 1;
	}

	char *input = read_file(argv[1]);
	Token *cur = tokenize(input);

	FILE *fout;
	fout = fopen("Output.xml", "w");
	if (fout == NULL) {
		fprintf(stderr, "出力ファイルをオープンできません");
		exit(1);
	}

	fprintf(fout, "<tokens>\n");

	while (cur->kind != TK_EOF) {
		if (cur->kind == TK_NUM)
			fprintf(fout, "<integerConstant>%ld</integerConstant>\n", cur->val);

		if (cur->kind == TK_IDENT)
			fprintf(fout, "<identifier>%s</identifier>\n", strndup(cur->loc, cur->len));

		if (cur->kind == TK_SYMBOL) {
			// XMLで<, >, &をエスケープするため
			if (!strncmp(cur->loc, "<", 1))
				fprintf(fout, "<symbol>&lt;</symbol>\n");

			if (!strncmp(cur->loc, ">", 1))
				fprintf(fout, "<symbol>&gt;</symbol>\n");

			if (!strncmp(cur->loc, "&", 1))
				fprintf(fout, "<symbol>&amp;</symbol>\n");

			if (strncmp(cur->loc, "<", 1) && strncmp(cur->loc, ">", 1) && strncmp(cur->loc, "&", 1))
				fprintf(fout, "<symbol>%s</symbol>\n", strndup(cur->loc, 1));
		}

		if (cur->kind == TK_RESERVED)
			fprintf(fout, "<keyword>%s</keyword>\n", strndup(cur->loc, cur->len));

		if (cur->kind == TK_STR)
			fprintf(fout, "<stringConstant>%s</stringConstant>\n", strndup(cur->contents, cur->cont_len));

		cur = cur->next;
	}

	fprintf(fout, "</tokens>");

	fclose(fout);
	return 0;
}
