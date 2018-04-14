#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BSIZE (PIPE_BUF/2)

char *p1 = "/tmp/f1";

typedef struct message {
	char fifo_name[BSIZE];
	char cmd_line[BSIZE];	
} msg_t;
