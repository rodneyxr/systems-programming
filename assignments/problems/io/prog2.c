#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFSZ 1024

int main(int nargs, char** argv) {
	char buf[BUFSZ];
	int fdin = STDIN_FILENO;
	int i;
	for (i = 1; i < nargs; i++) {
		fdin = open(argv[i], O_RDONLY);
		if (fdin == -1) {
			perror(argv[i]);
			exit(-1);
		}
		int n;
		while ((n = read(fdin, buf, BUFSZ)) > 0) {
			if (write(STDOUT_FILENO, buf, n) != n) {
				perror("write error");
				close(fdin);
				exit(-1);
			}
		}
		if (n == -1) {
			perror("read error");
			close(fdin);
			exit(-1);
		}
		close(fdin);
	}
	return 0;
}
