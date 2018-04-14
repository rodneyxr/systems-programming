#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include "logprt.h"

int fdout;
int fdin;
struct pcap_file_header *pcapfh;
struct my_pkthdr *pkthdr;

int main(int nargs, char** argv) {
	char buf[BUFSZ];
	int n;

	fdin = STDIN_FILENO;

	fdin = open(argv[1], O_RDONLY);
	if (fdin == -1) {
		sys_err(argv[1]);
	}

	pcapfh = malloc(sizeof(struct pcap_file_header));
	if (pcapfh == NULL)
		my_err("unable to malloc");
	n = read(fdin, pcapfh, sizeof(struct pcap_file_header));
	if (n == -1) {
		sys_err("read error");
	}

	switch (pcapfh->magic) {
	case PCAP_MAGIC:
		fprintf(stdout, "PCAP_MAGIC\n");
		break;
	case PCAP_SWAPPED_MAGIC:
		fprintf(stdout, "PCAP_SWAPPED_MAGIC\n");
		break;
	case PCAP_MODIFIED_MAGIC:
		fprintf(stdout, "PCAP_SWAPPED_MAGIC\n");
		break;
	case PCAP_SWAPPED_MODIFIED_MAGIC:
		fprintf(stdout, "PCAP_SWAPPED_MODIFIED_MAGIC\n");
		break;
	default:
		my_err("invalid file header");
	}

	/* print file header */
	fprintf(stdout, "Version major number = %d\n"
			"Version minor number = %d\n"
			"GMT to local correction = %d\n"
			"Timestamp accuracy = %d\n"
			"Snaplen = %d\n"
			"Linktype = %d\n",
			pcapfh->version_major, pcapfh->version_minor, pcapfh->thiszone,	pcapfh->sigfigs, pcapfh->snaplen, pcapfh->linktype);
	fprintf(stdout, "\n");

	int firsttime = 1;
	int packet = 0;
	unsigned int b_sec;
	int b_usec;
	unsigned int c_sec;
	int c_usec;
	do {
		/* read packet header */
		pkthdr = malloc(sizeof(struct my_pkthdr));
		if (pcapfh == NULL)
			my_err("unable to malloc");
		n = read(fdin, pkthdr, sizeof(struct my_pkthdr));
		if (n == 0) {
			free(pkthdr);
			break;
		}
		fprintf(stdout, "Packet %d\n", packet);

		if (firsttime) {
			firsttime = 0;
			b_sec = pkthdr->ts.tv_sec;
			b_usec = pkthdr->ts.tv_usec;
		}
		c_sec = (unsigned) pkthdr->ts.tv_sec - b_sec;
		c_usec = (unsigned) pkthdr->ts.tv_usec - b_usec;
		while (c_usec < 0) {
			c_usec += 1000000;
			c_sec--;
		}

		fprintf(stdout, "%05u.%06u\n"
				"Captured Packet Length = %d\n"
				"Actual Packet Length = %d\n",
				(unsigned) c_sec, (unsigned) c_usec, pkthdr->caplen, pkthdr->len);

		n = read(fdin, buf, pkthdr->caplen);

		packet++;
		free(pkthdr);
		fprintf(stdout, "\n");
	} while (n > 0);
	if (n == -1)
		sys_err("read error");

	free(pcapfh);
	close(fdin);
	return 0;
}

void my_err(const char* msg) {
	fprintf(stderr, "%s", msg);
	err();
}

void sys_err(const char* msg) {
	perror(msg);
	err();
}

void err() {
	free(pcapfh);
	free(pkthdr);
	close(fdin);
	exit(-1);
}
