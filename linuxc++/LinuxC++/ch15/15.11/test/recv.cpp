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
#include <netinet/if_ether.h>   //for ethhdr
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
void print_sockaddr_ll(struct sockaddr_ll *sa)  
{  
	if (sa == NULL)  
		return;  
	printf("sll_family:%d\n", sa->sll_family);  
	printf("sll_protocol:%#x\n", ntohs(sa->sll_protocol));  
	printf("sll_ifindex:%#x\n", sa->sll_ifindex);  
	printf("sll_hatype:%d\n", sa->sll_hatype);  
	printf("sll_pkttype:%d\n", sa->sll_pkttype);  
	printf("sll_halen:%d\n", sa->sll_halen);  
	printf("sll_addr:"); print_str16(sa->sll_addr, sa->sll_halen);  
}  
int main()  
{  
	int             result = 0, fd, n, count = 0;  
	char    buf[LEN];  
	struct sockaddr_ll      sa, sa_recv;  
	struct ifreq    ifr;  
	socklen_t       sa_len = 0;  
	char    if_name[] = "eno16777736";  
	struct ethhdr *eth; //定义以太网头结构体指针
  

	//create socket  
	fd = socket(PF_PACKET, SOCK_RAW, htons(0x8902));  
	if (fd < 0) {  
		perror("socket error\n");  
		return errno;  
	}  
	
	
	memset(&sa, 0, sizeof(sa));  
	sa.sll_family = PF_PACKET;  
	sa.sll_protocol = htons(0x8902);  
	
	// get flags   
	strcpy(ifr.ifr_name, if_name);  //必须先得到flags，才能再得到index
	result = ioctl(fd, SIOCGIFFLAGS, &ifr);  
	if (result != 0) {   
		perror("ioctl error, get flags\n");  
		return errno;  
	} 
	
	ifr.ifr_flags |= IFF_PROMISC;  
	// set promisc mode  
	result = ioctl(fd, SIOCSIFFLAGS, &ifr);  
	if (result != 0) {  
		perror("ioctl error, set promisc\n");  
		return errno;  
	}  
	
	result = ioctl(fd, SIOCGIFINDEX, &ifr);        //get index  
	if (result != 0) {  
		perror("ioctl error, get index\n");  
		return errno;  
	}  
	
 
	
	
	sa.sll_ifindex = ifr.ifr_ifindex;  
	result = bind(fd, (struct sockaddr*)&sa, sizeof(struct sockaddr_ll));  //bind fd  
	if (result != 0) {  
		perror("bind error\n");  
		return errno;  
	}  
	

	
  
      //recvfrom  
	while (1) {  
		memset(buf, 0, sizeof(buf));  
		n = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *)&sa_recv, &sa_len);  
	   
		if (n < 0) {  
			printf("recvfrom error, %d\n", errno);  
			return errno;  
		}  
		printf("******************* recvfrom msg %d ****************\n", ++count);  
		print_str16((unsigned char*)buf, n);  
		
		eth = (struct ethhdr*)buf;
	//从eth里提取目的mac、源mac、协议号
		printf("proto=0x%04x,dst mac addr:%02x:%02x:%02x:%02x:%02x:%02x\n", ntohs(eth->h_proto), eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
		printf("proto=0x%04x,src mac addr:%02x:%02x:%02x:%02x:%02x:%02x\n", ntohs(eth->h_proto), eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);

		print_sockaddr_ll(&sa_recv);  
		printf("sa_len:%d\n", sa_len);  
	}  
	return 0;  
}