#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/if_packet.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <unistd.h> //for close


typedef struct _iphdr //定义IP首部 
{ 
	unsigned char h_verlen; //4位首部长度+4位IP版本号 
	unsigned char tos; //8位服务类型TOS 
	unsigned short total_len; //16位总长度（字节） 
	unsigned short ident; //16位标识 
	unsigned short frag_and_flags; //3位标志位 
	unsigned char ttl; //8位生存时间 TTL 
	unsigned char proto; //8位协议 (TCP, UDP 或其他) 
	unsigned short checksum; //16位IP首部校验和 
	unsigned int sourceIP; //32位源IP地址 
	unsigned int destIP; //32位目的IP地址 
}IP_HEADER; 

typedef struct _udphdr //定义UDP首部
{
	unsigned short uh_sport;    //16位源端口
	unsigned short uh_dport;    //16位目的端口
	unsigned int uh_len;//16位UDP包长度
	unsigned int uh_sum;//16位校验和
}UDP_HEADER;

typedef struct _tcphdr //定义TCP首部 
{ 
	unsigned short th_sport; //16位源端口 
	unsigned short th_dport; //16位目的端口 
	unsigned int th_seq; //32位序列号 
	unsigned int th_ack; //32位确认号 
	unsigned char th_lenres;//4位首部长度/6位保留字 
	unsigned char th_flag; //6位标志位
	unsigned short th_win; //16位窗口大小
	unsigned short th_sum; //16位校验和
	unsigned short th_urp; //16位紧急数据偏移量
}TCP_HEADER; 

typedef struct _icmphdr {  
	unsigned char  icmp_type;  
	unsigned char icmp_code; /* type sub code */  
	unsigned short icmp_cksum;  
	unsigned short icmp_id;  
	unsigned short icmp_seq;  
	/* This is not the std header, but we reserve space for time */  
	unsigned short icmp_timestamp;  
}ICMP_HEADER;

void analyseIP(IP_HEADER *ip);
void analyseTCP(TCP_HEADER *tcp);
void analyseUDP(UDP_HEADER *udp);
void analyseICMP(ICMP_HEADER *icmp);


int main(void)
{
	int sockfd;
	IP_HEADER *ip;
	char buf[10240];
	ssize_t n;
	/* capture ip datagram without ethernet header */
	if ((sockfd = socket(PF_PACKET, SOCK_DGRAM, htons(ETH_P_IP))) == -1)
	{    
		printf("socket error!\n");
		return 1;
	}
	while (1)
	{
		n = recv(sockfd, buf, sizeof(buf), 0);
		if (n == -1)
		{
			printf("recv error!\n");
			break;
		}
		else if (n == 0)
			continue;
		//接收数据不包括数据链路帧头
		ip = (IP_HEADER *)(buf);
		analyseIP(ip);
		size_t iplen =  (ip->h_verlen & 0x0f) * 4;
		TCP_HEADER *tcp = (TCP_HEADER *)(buf + iplen);
		if (ip->proto == IPPROTO_TCP)
		{
			TCP_HEADER *tcp = (TCP_HEADER *)(buf + iplen);
			analyseTCP(tcp);
		}
		else if (ip->proto == IPPROTO_UDP)
		{
			UDP_HEADER *udp = (UDP_HEADER *)(buf + iplen);
			analyseUDP(udp);
		}
		else if (ip->proto == IPPROTO_ICMP)
		{
			ICMP_HEADER *icmp = (ICMP_HEADER *)(buf + iplen);
			analyseICMP(icmp);
		}
		else if (ip->proto == IPPROTO_IGMP)
		{
			printf("IGMP----\n");
		}
		else
		{
			printf("other protocol!\n");
		}        
		printf("\n\n");
	}
	close(sockfd);
	return 0;
}

void analyseIP(IP_HEADER *ip)
{
	unsigned char* p = (unsigned char*)&ip->sourceIP;
	printf("Source IP\t: %u.%u.%u.%u\n", p[0], p[1], p[2], p[3]);
	p = (unsigned char*)&ip->destIP;
	printf("Destination IP\t: %u.%u.%u.%u\n", p[0], p[1], p[2], p[3]);

}

void analyseTCP(TCP_HEADER *tcp)
{
	printf("TCP -----\n");
	printf("Source port: %u\n", ntohs(tcp->th_sport));
	printf("Dest port: %u\n", ntohs(tcp->th_dport));
}

void analyseUDP(UDP_HEADER *udp)
{
	printf("UDP -----\n");
	printf("Source port: %u\n", ntohs(udp->uh_sport));
	printf("Dest port: %u\n", ntohs(udp->uh_dport));
}

void analyseICMP(ICMP_HEADER *icmp)
{
	printf("ICMP -----\n");
	printf("type: %u\n", icmp->icmp_type);
	printf("sub code: %u\n", icmp->icmp_code);
}