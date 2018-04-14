#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "cmd.h"

int main(int nargs, char** argv) {
	/* create the fifo if it doesn't exist */
	if (mkfifo(CMD_FIFO, 0600) == -1)
		if (errno != EEXIST)
			sys_err("mkfifo");
	fprintf(stdout, "Server waiting for client to connect...\n");
	int cmdin = open(CMD_FIFO, O_RDONLY);
	if (cmdin == -1)
		sys_err(CMD_FIFO);

	fprintf(stdout, "Client connected.\n");
	int n;
	char buf[BUFSZ];
	while ((n = read(cmdin, buf, sizeof(buf))) > 0) {
		if (write(STDOUT_FILENO, buf, n) != n) {
			close(cmdin);
			sys_err("write");
		}
	}
	if (n == -1) {
		close(cmdin);
		sys_err("read");
	}
	
	close(cmdin);
	printf("Server finished.\n");
	return 0;
}

int sys_err(const char* msg) {
	perror(msg);
	exit(-1);
}
