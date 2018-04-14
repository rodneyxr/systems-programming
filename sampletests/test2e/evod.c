#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

void sys_err(const char* msg);

int main(int nargs, char** argv) {
	int pipefd[2];
	if (pipe(pipefd))
		sys_err("pipe");
	int i;
	for (i = 0; i < 40; i++) {
		switch(fork()) {
			case -1:
				sys_err("fork");
			case 0:
				if(getpid() % 2 == 0) { // if even
					if (write(pipefd[1], "one", 3) != 3)
						sys_err("write");
				} else { // if odd
					if (write(pipefd[1], "one", 3) != 3)
						sys_err("write");
					if (write(pipefd[1], "two", 3) != 3)
						sys_err("write");
				}
				exit(0);
		}
	}
	close(pipefd[1]);
	
	int n;
	char buf[3];
	while ( (n = read(pipefd[0], buf, 3)) > 0) {
		if (write(STDOUT_FILENO, buf, n) != n)
			sys_err("write");
		printf("\n");
	}
	if (n == -1)
		sys_err("read");
	close(pipefd[0]);
	return 0;
}

void sys_err(const char* msg) {
	perror(msg);
	exit(-1);
}
