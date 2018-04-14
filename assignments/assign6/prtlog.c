#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include "pcap.h"
#include "dnet.h"
#include "prtlog.h"

int fdin;
struct pcap_file_header *pcapfh;
struct my_pkthdr *pkthdr;
struct eth_hdr *ehdr;
struct arp_hdr *arphdr;
struct ip_hdr *iphdr;

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
			"Snaplen = %d\nLinktype = %d\n",
			pcapfh->version_major, pcapfh->version_minor, pcapfh->thiszone, pcapfh->sigfigs, pcapfh->snaplen, pcapfh->linktype);
	fprintf(stdout, "\n");

	int firsttime = 1;
	int packet = 0;
	int pksize = 0;
	unsigned int b_sec;
	int b_usec;
	unsigned int c_sec;
	int c_usec;
	char* s_etype;
	do {
		/* read packet header */
		pkthdr = malloc(sizeof(struct my_pkthdr));
		if (pcapfh == NULL)
			my_err("unable to malloc");
		n = read(fdin, pkthdr, sizeof(struct my_pkthdr));
		
		/* exit loop if no more data to read */
		if (n == 0) {
			free(pkthdr);
			break;
		}

		/* calculate time relative to first packet */
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
		
		pksize = pkthdr->caplen; /* set packet size */
		
		/* read ethernet header */
		ehdr = malloc(sizeof(struct eth_hdr));
		if (ehdr == NULL)
			my_err("unable to malloc");
		n = read(fdin, ehdr, sizeof(struct eth_hdr));

		pksize -= n; /* decrement packet size */
		u_short t;
		switch (t = ntohs(ehdr->eth_type)) { 
			case ETH_TYPE_PUP:
				s_etype = "PUP";
				break;
			case ETH_TYPE_IP:
				s_etype = "IP";
				iphdr = malloc(sizeof(struct ip_hdr));
				if (iphdr == NULL)
					my_err("failed fo malloc");
				n = read(fdin, iphdr, sizeof(struct ip_hdr));
				pksize -= n;
				break;
			case ETH_TYPE_ARP:
				s_etype = "ARP";
				arphdr = malloc(sizeof(struct arp_hdr));
                                if (arphdr == NULL)
                                        my_err("failed fo malloc");
                                n = read(fdin, arphdr, sizeof(struct arp_hdr));
				pksize -= n;
				break;
			case ETH_TYPE_REVARP:
				s_etype = "REVARP";
				break;
			case ETH_TYPE_8021Q:
				s_etype = "8021Q";
				break;
			case ETH_TYPE_IPV6:
				s_etype = "IPV6";
                                break;
			case ETH_TYPE_MPLS:
				s_etype = "MPLS";
                                break;
			case ETH_TYPE_MPLS_MCAST:
				s_etype = "MPLS_MCAST";
                                break;
			case ETH_TYPE_PPPOEDISC:
				s_etype = "PPPOEDISC";
                                break;
			case ETH_TYPE_PPPOE:
				s_etype = "PPPOE";
                                break;
			case ETH_TYPE_LOOPBACK:
				s_etype = "LOOPBACK";
                                break;
			default:
				s_etype = "UNRECOGNIZED";
		}

		/* read rest of packet */
		n = read(fdin, buf, pksize);

		/* print packet number */
                fprintf(stdout, "Packet %d\n", packet);
		/* print packet header */
                fprintf(stdout, "%05u.%06u\n"
				"Captured Packet Length = %d\n"
				"Actual Packet Length = %d\n",
				(unsigned) c_sec, (unsigned) c_usec, pkthdr->caplen, pkthdr->len);
		/* output ethernet header */
		fprintf(stdout, "Ethernet Header\n");
		/* print ethernet type */
		fprintf(stdout, "   %s\n", s_etype);
		
		if (t == ETH_TYPE_IP) {
			char* op;
			switch (iphdr->ip_p) {
				case IP_PROTO_ICMP:
					op = "ICMP";
					break;
				case IP_PROTO_IGMP:
					op = "IGMP";
					break;
				case IP_PROTO_TCP:
					op = "TCP";
					break;
				case IP_PROTO_UDP:
					op = "UDP";
					break;
				default:
					op = "UNRECOGNIZED";
			}
			fprintf(stdout, "      %s\n", op);
			free(iphdr);
		} else if (t == ETH_TYPE_ARP) {
			char* op;
			switch (ntohs(arphdr->ar_op)) {
				case ARP_OP_REQUEST:
					op = "Request";
					break;
				case ARP_OP_REPLY:
					op = "Reply";
					break;
				case ARP_OP_REVREQUEST:
					op = "Revrequest";
					break;
				case ARP_OP_REVREPLY:
					op = "Revreply";
					break;
				default:
					op = "UNRECOGNIZED";
			}
			fprintf(stdout, "      arp operation = Arp %s\n", op);
			free(arphdr);
		}

		packet++;
		free(ehdr);
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
	fprintf(stderr, "%s\n", msg);
	err();
}

void sys_err(const char* msg) {
	perror(msg);
	err();
}

void err() {
	free(pcapfh);
	free(pkthdr);
	free(ehdr);
	free(iphdr);
	free(arphdr);
	close(fdin);
	exit(-1);
}
