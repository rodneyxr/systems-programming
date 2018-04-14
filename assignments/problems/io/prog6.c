#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct product {
	int type;
	int number;
} prod_t;

int main(int nargs, char** argv) {
	int n;
	unsigned int u;
	int fdin = STDIN_FILENO;
	fdin = open(argv[1], O_RDONLY);
	prod_t product;

	while ((n = read(fdin, &product, sizeof(prod_t))) == sizeof(prod_t)) {
		fprintf(stdout, "Product\t%d\tQuantity\t%d\n", product.type, product.number);
		int x;
		for (x = 0; x < product.number; x++) {
			n = read(fdin, &u, sizeof(unsigned int));
		}
	}
	if (n == -1) {
		perror("read error");
		close(fdin);
		exit(-1);
	} else if (n > 0) {
		fprintf(stderr, "Incomplete Record\n");
		close(fdin);
		exit(-1);
	}

	close(fdin);
	return 0;
}
