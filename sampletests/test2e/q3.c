#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

struct command {
	char *cmd; /* name of the command */
	char *argv[10]; /* null terminated argv list of arguments to command */
};

int compose(struct command cmd1, struct command cmd2);
void sys_err(const char* msg);

int main(int nargs, char** argv) {
	struct command cmd1, cmd2;

	cmd1.cmd = "echo";
	cmd1.argv[0] = "echo";
	cmd1.argv[1] = "Hello, World!\n";

	cmd2.cmd = "cat";
	cmd2.argv[0] = "cat";

	//printf("%s\n%s\n", cmd1.cmd, cmd1.argv[0]);
	//printf("%s\n", cmd2.cmd);

	if (compose(cmd1, cmd2) == -1)
		sys_err("compose");

	return 0;
}

int compose(struct command cmd1, struct command cmd2) {
	int pipefd[2];
	if (pipe(pipefd))
		sys_err("pipe");
	//int fd;
	switch(fork()) {
		case -1:
			sys_err("fork");
		case 0:
			dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[1]);
			close(pipefd[0]);
			execvp(cmd1.cmd, cmd1.argv);
			sys_err("exec cmd1");
	}

	switch(fork()) {
		case -1:
			sys_err("fork");
		case 0:
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
			close(pipefd[1]);
			execvp(cmd2.cmd, cmd2.argv);
			sys_err("exec cmd2");
	}

	return 0;
}

void sys_err(const char* msg){
	perror(msg);
	exit(-1);
}
