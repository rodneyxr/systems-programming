#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) {
//	fork();
//	fork();
	printf("execing\n");
	execlp("echo" ,"echo", "hello world", NULL);
	perror("exec");
	printf("done\n");
	return 0;
}
