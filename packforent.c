#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

// Uber efficient bitstring processing ;)
unsigned char getbit(unsigned char *bits, unsigned long n) {
	return (bits[n / 8] & (unsigned char)pow(2, n % 8)) >> n % 8;
}

void setbit(unsigned char *bits, unsigned long n, unsigned char val) {
	bits[n / 8] =
	    (bits[n / 8] & ~(unsigned char)pow(2, n % 8)) | ((unsigned char)
							     pow(2,
								 n % 8) * val);
}

int main(void) {
	FILE *fp, *fp2;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	fp = fopen("rnd", "r");
	fp2 = fopen("rnd.bin", "w");

	if (fp == NULL)
		exit(EXIT_FAILURE);

	int i = 0;
	unsigned char tmp = 0;
	while ((read = getline(&line, &len, fp)) != -1) {
		
		setbit(&tmp, i, atoi(line));
		i++;

		if (i == 8) {
			fputc(tmp, fp2);
			fflush(fp2);
			i = 0;
		}
	}

	if (line)
		free(line);
	exit(EXIT_SUCCESS);
}
