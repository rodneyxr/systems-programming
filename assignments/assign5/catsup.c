#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFSZ 1024

void error(const char* errmsg);

char buf[BUFSZ];
int fdin1, fdin2, fdout;

int main(int nargs, char** argv) {
	/* Check for correct usage */
	if (nargs < 3 || nargs > 4) {
		fprintf(stderr, "Usage: catsup file1.in file2.in <file3.out>\n");
		exit(-1);
	}

	if (!strcmp(argv[1], "-") && !strcmp(argv[2], "-")) {
		fprintf(stderr, "Error: Only one of the first two files can be read from standard-in.\n");
		exit(-1);
	}

	/* Initialize file descriptors */
	fdin1 = STDIN_FILENO;
	fdin2 = STDIN_FILENO;
	fdout = STDOUT_FILENO;

	/* First file descriptor in */
	if (strcmp(argv[1], "-")) {
		fdin1 = open(argv[1], O_RDONLY);
		if (fdin1 < 0)
			error(argv[1]);
	}

	/* Second file descriptor in */
	if (strcmp(argv[2], "-")) {
		fdin2 = open(argv[2], O_RDONLY);
		if (fdin2 < 0)
			error(argv[2]);
	}

	/* File descriptor out */
	if (nargs == 4) {
		fdout = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0600);
		if (fdout < 0)
			error(argv[3]);
	}

	int n;
	while ((n = read(fdin1, buf, BUFSZ)) > 0) {
		if (write(fdout, buf, n) != n)
			error("Error: Failed to perform full write.\n");				
	}
	if (n == -1)
		error(argv[1]);
	
	while ((n = read(fdin2, buf, BUFSZ)) > 0) {
		if (write(fdout, buf, n) != n)
			error("Error: Failed to perform full write.\n");                        
	}
	if (n == -1)
		error(argv[2]);

	close(fdin1);
	close(fdin2);
	close(fdout);

	exit(0);
}

void error(const char* errmsg) {
	perror(errmsg);
	close(fdin1);
	close(fdin2);
	close(fdout);
	exit(-1);
}
