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
	fout = fopen("Output.txt", "w");
	if (fout == NULL) {
		fprintf(stderr, "出力ファイルをオープンできません");
		exit(1);
	}

	while (cur->kind != TK_EOF) {
		if (cur->kind == TK_NUM)
			fprintf(fout, "token_type: %s, value: %ld\n", STR(TK_NUM), cur->val);

		if (cur->kind == TK_IDENT)
			fprintf(fout, "token_type: %s, identifier: %s\n", STR(TK_IDENT), strndup(cur->loc, cur->len));

		if (cur->kind == TK_SYMBOL)
			fprintf(fout, "token_type: %s, symbol: %s\n", STR(TK_SYMBOL), strndup(cur->loc, 1));

		if (cur->kind == TK_RESERVED)
			fprintf(fout, "token_type: %s, keyword: %s\n", STR(TK_RESERVED), strndup(cur->loc, cur->len));

		if (cur->kind == TK_STR)
			fprintf(fout, "token_type: %s, string_literal: %s\n", STR(TK_STR), strndup(cur->contents, cur->cont_len));

		cur = cur->next;
	}

	fclose(fout);
	return 0;
}
