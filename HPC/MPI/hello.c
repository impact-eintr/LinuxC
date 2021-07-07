//hello_c.c
#include<mpi.h>
#include<stdio.h>
#include<string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void get(char *hname){
    struct hostent *hent;
    gethostname(hname, 128);//sizeof(hname)
    hent = gethostent();
    hent = gethostbyname(hname);
    printf("hostname: %s/naddress list: \n", hent->h_name);
}

int main(int argc,char *argv[])
{
    int my_rank;/*进程序号*/
    int p; /*进程总数*/
    int source;/*发送者序号*/
    int dest;/*接受者序号*/
    int tag=0;/*消息标签*/
    char message[1024];/*消息储存*/
    MPI_Status status;/*return status for*/

    /*启动MPI*/
    MPI_Init(&argc,&argv);
    /*查找进程号*/
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    /*查找进程总数*/
    MPI_Comm_size(MPI_COMM_WORLD,&p);

    if(my_rank!=0)
    {/*创建消息*/
        //得到本机的ip地址
        char hostname[128];
        get(hostname);
        sprintf(message,"Greeting from process %d,and ip is %s!",my_rank,hostname);
        dest=0;
        /*Use strlen+1 so that \0 gets transmitted*/
        MPI_Send(message,strlen(message)+1,MPI_CHAR,dest,tag,MPI_COMM_WORLD);
    }else{/*my rank ==0*/
        for(source=1;source<p;source++){
            MPI_Recv(message,100,MPI_CHAR,source,tag,MPI_COMM_WORLD,&status);
            printf("%s\n",message);
        }
        char hostname[128];
        get(hostname);
        printf("Greeting from process %d,and ip is %s!\n",my_rank,hostname);
    }
    /*关闭MPI*/
    MPI_Finalize();
}/*主函数结束*/
