#include <sys/epoll.h>


#ifndef EPOLL_H
#define EPOLL_H 
void addfd(int epollfd,int fd,bool enableET);
void epollHandler(epoll_event* event,int number,int epollfd,int listenfd);


#endif

