#include "IO/epoll.h"
#include "IO/socket_init.h"
#include "Thread/thread_pool.h"

#include <assert.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib> 
using std::cout;
using std::endl;

#define MAX_EVENT_NUMBER 1024


/*
 * 测试服务器的总体框架
 * 
 */
int main (int argc,char* argv[])
{
    if(argc <=2)
    {
        std::cout <<"ip + port" <<std::endl;
        exit(0);
    } 
    
     //服务端初始化 ，创建监听套接字,设置套接字的属性
     //argv[1]=ip ,  argv[2] = port,监听套接字的长度为 length
    int listenArrayLength = 10; 
    int listenfd = Server_init_tcp(argv,listenArrayLength);


     //epoll 初始化 
    epoll_event events[MAX_EVENT_NUMBER]; // 存放epoll 返回监听到发生的事件 
    int epollfd =epoll_create(5);
    assert(epollfd != -1);
    addfd(epollfd,listenfd,false);  // 对监听套接字 启用LT 触发方式
    
    // 线程池 初始化 
    
    tpool_create(3);  // 创建 具有 三个线程的线程池  
    while(1)  // 主线程 做eventloop 
    {
        int ret = epoll_wait(epollfd,events,MAX_EVENT_NUMBER,-1);
        if( ret < 0)
        {
            std::cout << "epoll error" <<std::endl;
            break;
        }
        if(ret == 0)
        {
            std::cout << "no event happened for temp" << std::endl;
            continue;
        }

        epollHandler(events,ret,epollfd,listenfd);  // IO/epoll.cpp
                
    }

    close(listenfd);
    tpool_destroy();  // 销毁线程池 
    sleep(2);  // 避免主线程先退出 

    return 0;
}



