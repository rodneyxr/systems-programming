#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
	char buf[1024];
	int n;
	while(( n = read(STDIN_FILENO, buf, sizeof(buf)) ) > 0)
		write(STDOUT_FILENO, buf, n);
	return 0;
}
