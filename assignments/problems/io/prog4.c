#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFSZ 1024

typedef struct vector {
	float x;
	float y;
	float z;
} vec_t;

typedef struct particle {
	float mass;
	vec_t pos;
	vec_t vel;
} part_t;

char* sVec(vec_t* v);

char buf[BUFSZ];
int main(int nargs, char** argv) {
	int fdin = STDIN_FILENO;
	fdin = open(argv[1], O_RDONLY);
	if (fdin == -1) {
		perror(argv[1]);
		exit(-1);
	}
	int n;

	part_t *p;
	int particle = 1;
	do {
		/* allocate space for the particle */
		p = malloc(sizeof(part_t));

		/* read the particle */
		n = read(fdin, p, sizeof(part_t));
		
		/* exit loop if no more data to read */
		if (n == 0)
			break;

		/* check if last record is complete */
                if (n != sizeof(part_t)) {
                        fprintf(stdout, "Error: Incomplete Record\n");
                        break;
                }

		/* output the particle */
		fprintf(stdout, "Particle\t%d\n", particle);
		fprintf(stdout, "\tmass\t%f\n", p->mass);
		fprintf(stdout, "\tpos\t%s\n", sVec(&(p->pos)));
		fprintf(stdout, "\tvel\t%s\n", sVec(&(p->vel)));

		/* free the particle */
		free(p);
		particle++;
	} while (n > 0);
	if (n == -1) {
		perror("Read Error");
		free(p);
		close(fdin);
		exit(-1);
	}
	
	free(p);
	close(fdin);
	return 0;
}

char* sVec(vec_t *v) {
	sprintf(buf, "(%f,%f,%f)", v->x, v->y, v->z);
	return buf;
}
