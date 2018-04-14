#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "np.h"

int main(int nargs, char** argv) {
	int f1, f2;
	char BUFFER[1024];
	msg_t msg;
	sprintf(msg.fifo_name, "/tmp/f-%d", getpid());
	if (mkfifo(msg.fifo_name, 0666)) {
		perror(msg.fifo_name);
		exit(-1);
	}

	if ((f1=open(p1, O_WRONLY)) < 0) {
		exit(-1);
	}

	write(STDOUT_FILENO, "CMD >", 5);
	int n;
	n = read(STDIN_FILENO, msg.cmd_line, BSIZE);
	write(f1, &msg, sizeof(msg));
	f2 = open(msg.fifo_name, O_RDONLY);
	while ((n = read(f2, BUFFER,sizeof(BUFFER))) > 0) {
		write(STDOUT_FILENO, BUFFER, n);
	}
	close(f1);
	close(f2);
}
