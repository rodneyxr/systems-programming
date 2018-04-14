#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFSZ 1024

int main(int nargs, char** argv) {
	char buf[BUFSZ];
	int fdin = STDIN_FILENO;
	fdin = open(argv[1], O_RDONLY);
	if (fdin == -1) {
		perror(argv[1]);
		exit(-1);
	}

	int n;
	while((n = read(fdin, buf, BUFSZ)) > 0) {
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
	exit(0);
}
