#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSZ 1024

int main(int nargs, char** argv) {
	char buf[BUFSZ];
	int fdin = STDIN_FILENO;
	
	fdin = open(argv[1], O_RDONLY);
	if (fdin == -1) {
		perror(argv[1]);
		exit(-1);
	}
	
	/* Print out the file on command line double spaced */
	int n;
	while((n = read(fdin, buf, BUFSZ)) > 0) {
		
		int i;
		for (i = 0; i < n; i++) {
			write(STDOUT_FILENO, buf + i, 1);
			if (buf[i] == '\n')
				write(STDOUT_FILENO, "\n", 1);
		}
	}
	if (n < 0) {
		perror("read error");
		close(fdin);
		exit(-1);
	}

	return 0;
}
