#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
int func1(int n) {
	int c_index = 1;
        int i;
	pid_t pid;
        for (i = 0; i < n; i++) {
                switch(pid = fork()) {
                        case -1:
                                fprintf(stderr, "fork error\n");
                                exit(-1);
				break;
                        case 0:
				sleep(3);
				fprintf(stdout, "%d\t%d\n",getpid(), i + 1);
				c_index++;
				exit(0);
		}	
        }
	printf("%d\t%d\n", getpid(), 0);
	return 0;
}
