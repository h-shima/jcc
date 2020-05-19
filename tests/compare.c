#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void compareFiles(FILE *fp1, FILE *fp2) {
	char actual = getc(fp1);
	char expected = getc(fp2);

	int error = 0, pos = 0, line = 1;

	while (actual != EOF && expected != EOF) {
		pos++;

		if (actual == '\n' && expected == '\n') {
			line++;
			pos = 0;
		}

		if (actual != expected) {
			error++;
			printf("Line number: %d\n", line);
			printf("Position: %d\n", pos);
		}

		// Fetch character until end-of-file
		actual = getc(fp1);
		expected = getc(fp2);
	}

	printf("Total Errors: %d\n", error);
}

int main(int argc, char **argv) {
	FILE *fp1 = fopen(argv[1], "r");
	FILE *fp2 = fopen(argv[2], "r");

	if (fp1 == NULL || fp2 == NULL) {
		printf("Error: Files not open");
		exit(1);
	}

	compareFiles(fp1, fp2);

	fclose(fp1);
	fclose(fp2);
	return 0;
}
