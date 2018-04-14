#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int users(void);
void sys_err(const char* msg);

int main(int nargs, char** argv) {
	int fdin = users();
	printf("%d\n", fdin);
	int n;
	char buf[1024];
	while (( n = read(fdin, buf, sizeof(buf))) > 0) {
		if (write(STDOUT_FILENO, buf, n) != n)
			sys_err("write");
	}
	if (n == -1)
		sys_err("read");
	close(fdin);
	return 0;
}

int users(void) {
	int pipefd[2];
	int fd;
	if (pipe(pipefd))
		sys_err("pipe");
	switch (fork()) {
		case -1:
			sys_err("fork");
		case 0:
			dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[0]);
			close(pipefd[1]);
			execlp("ls", "ls", NULL);
			sys_err("exec");
	}
	dup2(pipefd[0], fd);
	close(pipefd[0]);
	close(pipefd[1]);
	return fd;
}

void sys_err(const char* msg) {
	perror(msg);
	exit(-1);
}
