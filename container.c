#include "jcc.h"

char *remove_extension(char *filename) {
	char *p;
	char *dot;

	p = malloc(strlen(filename));
	strcpy(p, filename);

	dot = strrchr(p, '.');
	if (dot != NULL)
		*dot = '\0';

	return p;
}
