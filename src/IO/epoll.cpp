#include <assert.h>
#include <cstdlib>
#include <cstring> 
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>



#include <iostream> 
using std::cout;
using std::endl;

#include "socket_init.h"
#include "../Thread/thread.h"


#define MAX_EVENT_NUMBER  10

#define BUFFSIZE 4096
 
//int times_trigger = 0;

int SetNonblocking(int fd)
{

    int old_option = fcntl(fd,F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd,F_SETFL,new_option);

    return old_option;
}

void addfd(int epollfd,int fd,bool enable_ET)
{
    epoll_event event;
    /*
    struct epoll_event
    {
        uint32_t events;	[> Epoll events <]
        epoll_data_t data;	[> User data variable <]
    } __EPOLL_PACKE
    typedef union epoll_data
    {
    void *ptr;
    int fd;
    uint32_t u32;
    uint64_t u64;
    } epoll_data_t;
    */
    event.data.fd = fd;
    event.events = EPOLLIN;
    if(enable_ET)
    {
        event.events |=EPOLLET;
    }

    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);

    SetNonblocking(fd);
}


/*
 *事件的触发模式是ET，使得事件被触发后，要将相应的socket 的数据接收完整
 */
void  epollHandler(epoll_event* events,int number,int epollfd,int listenfd)
{
    char* buf = (char*)malloc(BUFFSIZE);
    for(int i = 0;i < number;i++)
    {
        int sockfd = events[i].data.fd;
        if(sockfd == listenfd)
        {
            int connfd = Get_socket_for_client(listenfd);
            addfd(epollfd,connfd,true);    // 对已经被accept的套接字，开启ET模式
        }
        else if(events[i].events & EPOLLIN) // 可读事件
        {
            memset(buf,0,BUFFSIZE);
            // 循环读取出所有的数据
            int n = 0;
            ssize_t  nread;
            while ((nread = recv(sockfd, buf + n,1024,0)) > 0) {
                //cout << buf << endl;  // test the string server get
                n += nread;
            } // 数据被全部读出
            if (nread == -1 && errno != EAGAIN) {
                cout << "read error,error no is :"<< errno <<endl;
            }     
           /*
            *将数据进行封装成DataToThread的结构体 
            */
            DataToThread* data = (DataToThread*)malloc(sizeof(DataToThread));
            data->fd = sockfd;
            data->json = buf; 

            // 创建一个线程 讲获取到数据传给线程 
                                   
             throwItTOThread(data);
        }
    }    
}

/*
 * unit test
 * @luo yuping
 */



 
//int main (int argc,char* argv[])
//{
    //if(argc <=2)
    //{
        //std::cout <<"ip + port" <<std::endl;
        //exit(0);
    //} 
    
    /*
     * 服务端初始化 ，创建监听套接字,设置套接字的属性
     * argv[1]=ip ,  argv[2] = port,监听套接字的长度为 length
     */  
    //int listenArrayLength = 10; 
    //int listenfd = Server_init_tcp(argv,listenArrayLength);


    /*
     * epoll 初始化 
     */
    //epoll_event events[MAX_EVENT_NUMBER]; // 存放epoll 返回监听到发生的事件 
    //int epollfd =epoll_create(5);
    //assert(epollfd != -1);
    //addfd(epollfd,listenfd,true);

    //while(1)
    //{
        //int ret = epoll_wait(epollfd,events,MAX_EVENT_NUMBER,-1);
        //if( ret < 0)
        //{
            //std::cout << "epoll error" <<std::endl;
            //break;
        //}
        //if(ret == 0)
        //{
            //std::cout << "no event happened for temp" << std::endl;
            //continue;
        //}

        ////ModeLT(events,ret,epollfd,listenfd);
        //ModeET(events,ret,epollfd,listenfd);
    //}

    //close(listenfd);
    //return 0;
//}







