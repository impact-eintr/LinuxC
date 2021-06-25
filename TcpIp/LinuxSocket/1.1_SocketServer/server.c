#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
//#include <sys/types.h>
//#include <netinet/ip.h>
//#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>


int main(){
  int server_fd, client_fd;
  int server_len, client_len;
  struct sockaddr_in server_address;
  struct sockaddr_in client_address;

  char back[] = {"I'm a server"};

  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  server_address.sin_family = AF_INET;


}
