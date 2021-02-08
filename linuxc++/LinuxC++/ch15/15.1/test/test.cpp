#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/if_ether.h>
#include <bits/ioctls.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <errno.h>
#include <string.h> //bzero
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h> //for close
#include <arpa/inet.h>//htons
#include <stdio.h>
#include <stdlib.h>//EXIT_FAILURE

#define IFRNAME   "eno16777736"
 

unsigned char dest_mac[6] = { 0 };

int main(int argc, char **argv)
{
	int i, datalen;
	int sd;

	struct sockaddr_ll device;
	struct ifreq ifr;

	bzero(&ifr, sizeof(struct ifreq));

	if ((sd = socket(PF_PACKET, SOCK_DGRAM, htons(ETH_P_ALL))) < 0) 
	{
		printf("socket() failed to get socket descriptor for using ioctl()");
		return (EXIT_FAILURE);
	}
	memcpy(ifr.ifr_name, IFRNAME, sizeof(struct ifreq));
	if (ioctl(sd, SIOCGIFHWADDR, &ifr) < 0) {
		printf("ioctl() failed to get source MAC address");
		return (EXIT_FAILURE);
	}
	close(sd);

	memcpy(dest_mac, ifr.ifr_hwaddr.sa_data, 6);
	
	printf("mac addr:%02x:%02x:%02x:%02x:%02x:%02x\n", dest_mac[0], dest_mac[1], dest_mac[2], dest_mac[3], dest_mac[4], dest_mac[5]);

	return 0;
}