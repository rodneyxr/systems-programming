/* magic constants for various pcap file fypes */
#define PCAP_MAGIC                      0xa1b2c3d4
#define PCAP_SWAPPED_MAGIC              0xd4c3b2a1
#define PCAP_MODIFIED_MAGIC             0xa1b2cd34
#define PCAP_SWAPPED_MODIFIED_MAGIC     0x34cdb2a1

struct timev {
	unsigned int tv_sec;
	unsigned int tv_usec;
};

/* data prefixing each packet */
struct my_pkthdr {
	struct timev ts; 	/* time stamp */
	int caplen; 		/* captured length of the packet */
	int len; 		/* actual length of the packet */
};

#define BUFSZ 4096

void my_err(const char* msg);
void sys_err(const char* msg);
void err(void);
