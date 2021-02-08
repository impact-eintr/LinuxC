#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, const char * argv[])
{

    struct in_addr ia;
    inet_aton("172.16.2.6", &ia);
    printf("ia.s_addr=0x%x\n",ia.s_addr);
    printf("real_ip=%s\n",inet_ntoa(ia));
    return 0;
}
