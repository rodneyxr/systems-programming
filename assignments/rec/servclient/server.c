#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "np.h"

void sys_err(const char* msg);

int main(int nargs, char** argv) {
	int f1, f2, n, fin;
	msg_t msg;
	int pfifo;
	mkfifo(pfifo, 0600);
	f1 = open(pfifo, O_RDONLY);
	if (f1 == -1)
		sys_err("open");
	while( (n = read(f1, &msg, sizeof(msg))) > 0) {
		do {
			f2 = open(msg.f2_name, O_WRONLY);
			if (f2 == -1)
				sys_err(msg.f2_name);
			fin = popen(msg.cmd_line, "r");
			while( (n = read(fileno(fin), BUFFER, PIPE_BUF)) > 0) {
				write(f2, BUFFER, n);
			}
			pclose(fin);
			close(f2);
		} while (1);
	}
	return 0;
}

void sys_err(const char* msg) {
	perror(msg);
	exit(-1);
}
