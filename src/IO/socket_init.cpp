#include "socket_init.h"

#include <sys/types.h> 
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h> 
#include <assert.h>

#include <iostream> 
using namespace std;


void print_Client_Info(struct sockaddr_in* clientaddr)
{
    unsigned long int port = ntohl(clientaddr->sin_port);
    char ipv4[20] ;
    memset(ipv4,0,20);
    //strcpy(AF_INET,inet_ntop(clientaddr->sin_addr.s_addr,ipv4,20));
    strcpy(ipv4,inet_ntoa(clientaddr->sin_addr));
    std::cout << "client:"<<"ip:"<<ipv4 << "port:" <<port <<std::endl;
}



int Server_init_tcp(char* argv[],int listenArrayLength)
{ 
    const char* ip = argv[1];
    int port = atoi(argv[2]);
    struct sockaddr_in address;
    memset(&address,0,sizeof(address));

    inet_pton(AF_INET,ip,&address.sin_addr);

    address.sin_port = htons(port);

    address.sin_family = PF_INET;

    int sock = socket(PF_INET,SOCK_STREAM,0);

    assert( sock != -1);
    /*
     * 避免time_wait,以防客户端先强制退出后，服务端进入time_wait状态后，重新启动后，无法立即绑定端口  
     */
    int option;
    int optlen = sizeof(option);
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,(void*)&option,optlen);

    /*
     * 关闭  nagle 算法
     */
    //option = 1;
    //setsockopt(sock,IPPROTO_TCP,TCP_NODELAY,(void*)&option,sizeof(option));

    int ret = bind(sock,(struct sockaddr *)&address,sizeof(address));

    assert(ret != -1);

    ret = listen(sock,listenArrayLength);

    assert(ret != -1);
    return sock;
}


int Client_init_tcp(char* argv[])
{ 
    const char* ip = argv[1];
    unsigned short port = atoi(argv[2]);

    struct sockaddr_in address;
    memset(&address,0,sizeof(address));
    address.sin_port = htons(port);
    address.sin_family = PF_INET;
    inet_pton(AF_INET,ip,&address.sin_addr);

    int sock = socket(PF_INET,SOCK_STREAM,0); //  创建一个tcp ipv4  的插口
    assert( sock != -1);
    int result= connect(sock,(struct sockaddr*)&address,sizeof(address));
    assert(result >=0);
    return sock;
} 

int Get_socket_for_client(int listen_socket)
{
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    int connfd = accept(listen_socket,(struct sockaddr*)&client_addr,&len);
    if(connfd < 0)
    { 
        return -1;
         
    } 

    print_Client_Info(&client_addr);  // 打印接入的socket的信息

    return connfd;
}

 

