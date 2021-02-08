  
#include <net/if.h>       /* for ifconf */  
#include <linux/sockios.h>    /* for net status mask */  
#include <netinet/in.h>       /* for sockaddr_in */  
#include <sys/socket.h>  
#include <sys/types.h>  
#include <sys/ioctl.h>  
#include <stdio.h>  
#include <unistd.h> //for close
#include <arpa/inet.h>
#include <string.h>
#define MAX_INTERFACE   (16)  
      
void port_status(unsigned int flags);  
      
/* set == 0: do clean , set == 1: do set! */  
int set_if_flags(char *pif_name, int sock, int status, int set)  
{  
	struct ifreq ifr;  
	int ret = 0;  
          
	strncpy(ifr.ifr_name, pif_name, strlen(pif_name) + 1);  
	ret = ioctl(sock, SIOCGIFFLAGS, &ifr);  
	if (ret)  
		return -1;  
	/* set or clean */    
	if (set)  
		ifr.ifr_flags |= status;  
	else   
		ifr.ifr_flags &= ~status;  
	/* set flags */  
	ret = ioctl(sock, SIOCSIFFLAGS, &ifr);  
	if (ret)  
		return -1;  
          
	return 0;  
}  
      
int get_if_info(int fd)  
{  
	struct ifreq buf[MAX_INTERFACE];      
	struct ifconf ifc;  
	int ret = 0;  
	int if_num = 0;  
      
	ifc.ifc_len = sizeof(buf);  
	ifc.ifc_buf = (caddr_t) buf;  
          
	ret = ioctl(fd, SIOCGIFCONF, (char*)&ifc);  
	if (ret)  
	{  
		printf("get if config info failed");  
		return -1;  
	}  
	/* 网口总数 ifc.ifc_len 应该是一个出入参数 */      
	if_num = ifc.ifc_len / sizeof(struct ifreq);  
	printf("interface num is interface = %d\n", if_num);  
	while (if_num-- > 0)  
	{  
		printf("net device: %s\n", buf[if_num].ifr_name);     
		/* 获取第n个网口信息 */  
		ret = ioctl(fd, SIOCGIFFLAGS, (char*)&buf[if_num]);  
		if (ret)  
			continue;  
      
        /* 获取网口状态 */  
		port_status(buf[if_num].ifr_flags);  
              
		/* 获取当前网卡的ip地址 */  
		ret = ioctl(fd, SIOCGIFADDR, (char*)&buf[if_num]);  
		if (ret)  
			continue;  
		printf("IP address is: \n%s\n", inet_ntoa(((struct sockaddr_in *)(&buf[if_num].ifr_addr))->sin_addr));  
      
        /* 获取当前网卡的mac */  
		ret = ioctl(fd, SIOCGIFHWADDR, (char*)&buf[if_num]);  
		if (ret)  
			continue;  
      
		printf("%02x:%02x:%02x:%02x:%02x:%02x\n\n",  
			(unsigned char)buf[if_num].ifr_hwaddr.sa_data[0],  
			(unsigned char)buf[if_num].ifr_hwaddr.sa_data[1],  
			(unsigned char)buf[if_num].ifr_hwaddr.sa_data[2],  
			(unsigned char)buf[if_num].ifr_hwaddr.sa_data[3],  
			(unsigned char)buf[if_num].ifr_hwaddr.sa_data[4],  
			(unsigned char)buf[if_num].ifr_hwaddr.sa_data[5]);  
	}  
}  
      
void port_status(unsigned int flags)  
{  
	if (flags & IFF_UP)    
	{  
		printf("is up\n");        
	}  
	if (flags & IFF_BROADCAST)     
	{  
		printf("is broadcast\n");     
	}  
	if (flags & IFF_LOOPBACK)      
	{  
		printf("is loop back\n");     
	}  
	if (flags & IFF_POINTOPOINT)   
	{  
		printf("is point to point\n");    
	}  
	if (flags & IFF_RUNNING)   
	{  
		printf("is running\n");   
	}  
	if (flags & IFF_PROMISC)   
	{  
		printf("is promisc\n");   
	}  
}  
      
int main()  
{  
	int fd;  
      
	fd = socket(AF_INET, SOCK_DGRAM, 0);  
	if (fd > 0)  
	{  
		get_if_info(fd);  
		close(fd);  
	}  
      
	return 0;  
}  
