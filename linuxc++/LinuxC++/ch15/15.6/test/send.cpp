#include <stdio.h>  
#include <string.h>  
#include <errno.h>  
#include <sys/types.h>   
#include <sys/socket.h>  
#include <netpacket/packet.h>  
#include <net/if.h>  
#include <net/if_arp.h>  
#include <sys/ioctl.h>  
#include <arpa/inet.h> //for htons
  
#define LEN     60  
  
  
void print_str16(unsigned char buf[], size_t len)  
{  
	int     i;  
	unsigned char   c;  
	if (buf == NULL || len <= 0)  
		return;  
	for (i = 0; i < len; i++) {  
		c = buf[i];  
		printf("%02x", c);  
	}  
	printf("\n");  
}  
int main()  
{  
	int             result = 0;  
	int             fd, n, count = 3, nsend = 0;  
	char    buf[LEN];  
	struct sockaddr_ll      sa;  
	struct ifreq    ifr;  
	char    if_name[] = "eno16777736";  
	char    dst_mac[6] = { 0x00,0x0c,0x29,0x3d,0x94,0x31 };// {0x00, 0x0c, 0x29, 0x3d, 0x94, 0x13};//{ 0x00, 0x50, 0xC2, 0x25, 0x8A, 0x20 };  
	char    src_mac[6];  
	short   type = htons(0x8902);  
  
	memset(&sa, 0, sizeof(struct sockaddr_ll));  
	memset(buf, 0, sizeof(buf));  
  
      //create socket  
	fd = socket(PF_PACKET, SOCK_RAW, htons(0x8902));  
	if (fd < 0) {  
		printf("socket error, %d\n", errno);  
		return errno;  
	}  
  
      //get index  
	strcpy(ifr.ifr_name, if_name);  
	result = ioctl(fd, SIOCGIFINDEX, &ifr);  
	if (result != 0) {  
		printf("get mac index error, %d\n", errno);  
		return errno;  
	}  
	sa.sll_ifindex = ifr.ifr_ifindex;  
  
      //get mac  
	result = ioctl(fd, SIOCGIFHWADDR, &ifr);  
	if (result != 0) {  
		printf("get mac addr error, %d\n", errno);  
		return errno;  
	}  
	memcpy(src_mac, ifr.ifr_hwaddr.sa_data, 6);  
  
      //set buf  
	memcpy(buf, dst_mac, 6);  
	memcpy(buf + 6, src_mac, 6);  
	memcpy(buf + 12, &type, 2);  
  
	print_str16((unsigned char*)buf, sizeof(buf));  
	//sendto  
	while (count-- > 0) {  
		n = sendto(fd, buf, sizeof(buf), 0, (struct sockaddr *)&sa, sizeof(struct sockaddr_ll));  
		if (n < 0) {  
			printf("sendto error, %d\n", errno);  
			return errno;  
		}  
		printf("sendto msg %d, len %d\n", ++nsend, n);  
	}  
	return 0;  
} 