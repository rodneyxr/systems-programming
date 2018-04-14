/**
 * Quiz 3 -> due monday 10th
 * Create a program that creates 2 children and have both childs send one
 * message back and forth three times.
 **/

/*
   Then child0 and child1 will put their read file descriptors of their pipe
   in fdin and the write file descriptors in fdout.  Then in the common code
   child0 writes a message to fdout.  Then loop the appropriate number of
   times where the current process reads the message from fdin and writes
   to fdout.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int nargs, char** argv) {
	pid_t pid, mpid, opid, parent;
	parent = getpid();
	int pipe0[2], pipe1[2], fdin, fdout;
	int ndx = 0;
	if (pipe(pipe0) || pipe(pipe1)) {
		perror("pipe");
		exit(-1);
	}

	int i;
	printf("Parent Process: %d\n\n", parent);
	for (i = 0; i < 2; i++) {
		switch ( pid = fork() ) {
			case -1: // Fork Error
				perror("fork");
				exit(-1);
			case 0: // child
				if (ndx == 0) {
					close(pipe0[1]);
					close(pipe1[0]);
					fdin = pipe0[0];
					fdout = pipe1[1];
				} else if (ndx == 1) {
					close(pipe0[0]);
					close(pipe1[1]);
					fdin = pipe1[0];
					fdout = pipe0[1];
				}
				break;
			default:
				ndx++;
		}
		if (pid == 0) break;
	}
	pid_t cpid = getpid();
	if (pid == 0)
		for (i = 0; i < 5; i++) {
			mpid = getpid();
			if (ndx == 0) {
				fdout = pipe1[1];
				fdin = pipe0[0];
				write(fdout, &mpid, sizeof(pid_t));
				if (i > 0) printf("pid %d: My relative has pid %d\n", cpid, opid);
			}
			read(fdin, &opid, sizeof(pid_t));
			if (ndx == 1) {
				fdout = pipe0[1];
				fdin = pipe0[1];
				write(fdout, &mpid, sizeof(pid_t));
				printf("pid %d: My relative has pid %d\n", cpid, opid);
			}
		}
	if (cpid != parent) exit(0);

	// wait for children to finish
	wait(NULL);
	wait(NULL);

	// parent sends signal that program is finished
	printf("\npid = %d, done.\n", cpid);
	return 0;
}
