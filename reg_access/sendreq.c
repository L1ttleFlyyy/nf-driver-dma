/* ****************************************************************************
 * $Id: regread.c 2267 2007-09-18 00:09:14Z grg $
 *
 * Module: regread.c
 * Project: NetFPGA 2 Register Access
 * Description: Reads a register
 *
 * Change history:
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<string.h>

#include <net/if.h>

#include "/usr/local/netfpga/lib/C/common/nf2.h"
#include "/usr/local/netfpga/lib/C/common/nf2util.h"

#define PATHLEN		80

#define DEFAULT_IFACE	"nf2c0"

/* Global vars */
static struct nf2device nf2;
static int verbose = 0;
static int force_cnet = 0;


void sendreqest(struct nf2device *nf2);

int main(int argc, char *argv[])
{
	unsigned val;

	nf2.device_name = DEFAULT_IFACE;


	// Open the interface if possible
	if (check_iface(&nf2))
	{
		exit(1);
	}
	if (openDescriptor(&nf2))
	{
		exit(1);
	}

	// Read the registers
	sendreqest(&nf2);

	closeDescriptor(&nf2);

	return 0;
}

/*
 * Read the register(s)
 */
void sendreqest(struct nf2device *nf2)
{
	int ret;
	struct ifreq ifreq;
	
	strncpy(ifreq.ifr_ifrn.ifrn_name, nf2->device_name, IFNAMSIZ);

	if((ret = ioctl(nf2->fd, SIOCSENDREQ, &ifreq)) != 0) {
		printf("FAILED.\n");
		printf("ret=%d\n", ret);
	}
}
