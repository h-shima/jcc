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
	Token *tok = tokenize(input);
	Node *node = parse(tok);

	// Traverse the AST to emit VM code.
	codegen(node);
	return 0;
}
