#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PNUM 9

void sendmail(int fd[PNUM][2], int ndx);
void checkmail(int fd[PNUM][2], int ndx);
void sys_err(const char* msg);
char buf[32];

int main(int nargs, char** argv) {
	pid_t pid;
	int fd[PNUM][2];
	int i, x, ndx = 0;

	/* create 9 pipes for 9 processes (0-8) */
	for (i = 0; i < PNUM; i++)
		if (pipe(fd[i])) sys_err("pipe");

	/* create the other 8 process and assign their index */
	for (i = 1; i < PNUM; i++) {
		switch(pid = fork()) {
			case -1:
				sys_err("fork");
			case 0:
				ndx = i; /* set child's ndx*/
				close(fd[ndx][1]); /* close write end to its own pipe */
				/* close all read ends of each pipe except for its own */
				for (x = 0; x < PNUM; x++)
					if (x != ndx)
						close(fd[x][0]);
		}
		if (!pid) break;
	}

	/* same for original process */
	if (ndx == 0) {
		close(fd[ndx][1]); /* close write end to its own pipe */
		for (i = 0; i < PNUM; i++) /* close all read ends of each pipe except for its own */
			if (i != ndx)
				close(fd[i][0]);
	}

	/* common code */

	/* send 13 random messages to other processes */
	sendmail(fd, ndx);
	/* close all write ends of pipes to other processes */
	for (i = 0; i < PNUM; i++)
		if (i != ndx) /* doesn't need to close its own because its already been closed */
			close(fd[i][1]);

	/* read any data in the processes own pipe */
	checkmail(fd, ndx);
	/* close read end of its own pipe */
	close(fd[ndx][0]);

	return 0;
}

void sendmail(int fd[PNUM][2], int ndx) {
	int n, num, j;
	srand(9872371 * ndx);
	int rng = RAND_MAX / PNUM;
	rng *= PNUM;
	for (n = 0; n < 13; n++) {
		do {
			num = rand();
			j = num % PNUM;
		} while (num >= rng || ndx == j);

		/* send message with ndx to process j */
		sprintf(buf, "process%d", ndx);
		if (write(fd[j][1], buf, sizeof(buf)) != sizeof(buf))
			sys_err("write");
	}
}

void checkmail(int fd[PNUM][2], int ndx) {
	int n;
	while ( (n = read(fd[ndx][0], buf, sizeof(buf))) > 0 ) {
		fprintf(stdout, "process%d has received a message from %s\n", ndx, buf);
	}
	if (n == -1)
		sys_err("read");
}

void sys_err(const char* msg) {
	perror(msg);
	exit(-1);
}
