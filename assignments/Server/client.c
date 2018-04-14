#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include "cmd.h"

int main(int nargs, char** argv) {
	int cmdout;
	cmdout = open(CMD_FIFO, O_WRONLY);
	printf("Client started.\n");

	int n;
	char buf[BUFSZ];
	while ( (n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
		if (write(cmdout, buf, n) != n) {
			close(cmdout);
			sys_err("read");
		}
	}
	if (n == -1) {
		close(cmdout);
		sys_err("read");
	}
	
	printf("Client finished.\n");
	return 0;
}

int sys_err(const char* msg){
	perror(msg);
	exit(-1);
}
