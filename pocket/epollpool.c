#include <fcntl.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>
#include <sys/epoll.h>
#include "cJSON.h"
#include "head.h"

#define MAX_EVENT_NUMBER 512
#define BUF_SIZE 1024

//extern  cJSON * cJSON_Prase (const char *p) ;

//extern  cJSON * cJSON_GetObjectItem (cJSON * ,const char *) ;

typedef struct tpool_work
{
    void *(*routine)(void *);
    void *arg;
    struct tpool_work *next;
}tpool_work_t;

typedef struct tpool
{
    int shutdown;
    int max_thr_num;
    pthread_t *thr_id;

    tpool_work_t *queue_head;

    pthread_mutex_t queue_lock;// 

    pthread_cond_t queue_ready;  // 定义的条件变量 

}tpool_t;

int tpool_create(int max_thr_num);

void *func(void *arg);

void tpool_destroy();

int tpool_add_work(void *(*routine)(void*),void *arg);


int listenFd;


static tpool_t *tpool = NULL; // 线程池管理数据结构 

int setnonblocking(int fd)
{
    int old_option = fcntl(fd,F_GETFL);

    int new_option = old_option | O_NONBLOCK; //| EPOLLONESHOT;

    fcntl(fd,F_SETFL,new_option);

    return old_option;
}

void addfd(int epollfd,int fd)
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;// | EPOLLONESHOT;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
    setnonblocking(fd);
}

static void *thread_routine(void *arg)
{
    tpool_work_t *work;
    while(1)
    {
        pthread_mutex_lock(&tpool->queue_lock);     // 从任务队列里取走任务 要加锁 

        while(tpool->queue_head == NULL && !tpool->shutdown) //
        {
            printf("ok1\n");   
            pthread_cond_wait(&tpool->queue_ready,&tpool->queue_lock);//
        }

        if(tpool->shutdown)
        {
            printf("hha\n");
            pthread_mutex_unlock(&tpool->queue_lock); 
            pthread_exit(NULL);
        }

        // 新的任务

        work = tpool->queue_head;

        tpool->queue_head = tpool->queue_head->next;  // 把任务取走  ，任务指针后移 

        pthread_mutex_unlock(&tpool->queue_lock);

        work->routine(work->arg);     // 调用任务函数 
        free(work);             // 释放节点 避免内存泄露 

        printf("ok2\n");   
    }
    return NULL;
}


int tpool_create(int max_thr_num)  //
{
    int i;

    tpool =(tpool_t *)calloc(1,sizeof(tpool_t));
    if(!tpool)
    {
        printf("error\n");
        exit(1);
    }
    tpool->max_thr_num = max_thr_num;
    tpool->shutdown = 0;
    tpool->queue_head = NULL; 

    pthread_mutex_init(&tpool->queue_lock,NULL);
    pthread_cond_init(&tpool->queue_ready,NULL);

    tpool->thr_id = (pthread_t *)calloc(max_thr_num,sizeof(pthread_t));
    for(i = 0;i < max_thr_num;++i)
    {
        pthread_create(&tpool->thr_id[i],NULL,thread_routine,NULL);
    }
 /*   for(i = 0;i < max_thr_num;++i)
    {
        pthread_join(tpool->thr_id[i],NULL);
    }*/
    return 0;

}

void tpool_destroy()
{
    int i;
    tpool_work_t *member;
    
    tpool->shutdown = 1;
    pthread_mutex_lock(&tpool->queue_lock); 
    pthread_cond_broadcast(&tpool->queue_ready);
    pthread_mutex_unlock(&tpool->queue_lock);

    for(i = 0;i < tpool->max_thr_num;i++)
    {
        pthread_join(tpool->thr_id[i],NULL);
    }
    free(tpool->thr_id);

    while(tpool->queue_head)
    {
        member = tpool->queue_head;
        tpool->queue_head = tpool->queue_head->next;
        free(member);
    }

    pthread_mutex_destroy(&tpool->queue_lock);
    pthread_cond_destroy(&tpool->queue_ready);
    free(tpool);
}

int tpool_add_work(void *(*routine)(void *),void *arg)
{
    tpool_work_t *work, *member;  

    work = (tpool_work_t *)malloc(sizeof(tpool_work_t));  // 函数链表

    work->routine = routine;  // create a node(type is tpool_work_t) 
    work->arg = arg;
    work->next = NULL;

    pthread_mutex_lock(&tpool->queue_lock);  // 任务队列的插入 要加锁 

    member = tpool->queue_head;  // 链表的头节点

    if(member == NULL)
    {
        tpool->queue_head = work;
    }
    else 
    {
        while(member->next)
        {
            member = member->next;
        }
        member->next = work;
    }
    // insert the new fun node into the fun_list 

    pthread_cond_signal(&tpool->queue_ready); //inform the work thread to work
 
    pthread_mutex_unlock(&tpool->queue_lock);  

    return 0;
}

void choose(int fd,char *p)  // 可重入的函数 
{
    cJSON *pJson = cJSON_Parse((const char *)p);
    int ret = -1;
    int a = -1;
    if(NULL == pJson)
    {
        printf("Json parse error\n");
        return ;
    }
    cJSON *pSub = cJSON_GetObjectItem(pJson,"funType");
    a = pSub->valueint;
    printf("%d\n",a);
    switch(a)
    {
        case 10:
            ret = login(fd,p);
            break;
        case 20:
            ret = regist(fd,p);
            break;
    /*    case 30:
            ret = sync(fd,p);
            break;
        case 40:
            ret = deleteInfo(fd,p);
            break;
        case 50:
            ret = addInfo(fd,p);
            break;
        case 60:
            ret = queryInfo(fd,p);*/
        default:
            printf("type error\n");
    }
    if(ret == ERROR)
    {
        printf("switch error\n");
    }
    free(p);  //  
}

void *test(void *arg)
{
    printf("fuck you!\n");
}

void *func(void *arg)
{
    int fd;
    int ret;
    int connfd = *(int *)arg;
    int length = BUF_SIZE;
    char buf[BUF_SIZE];

    char *p = (char *)malloc(BUF_SIZE);
    memset(p,'\0',BUF_SIZE);
    while(1)
    { 
        memset(buf,'\0',BUF_SIZE);
        ret = recv(connfd,buf,BUF_SIZE,0);

        if(ret < 0)
        {
            if((errno == EAGAIN) || (errno == EWOULDBLOCK))
            {
                break;
            }

            close(connfd);
            break;
        }
        else if(ret == 0)
        {
            close(connfd);
        }
        if(strlen(buf) <= BUF_SIZE)
        {
            strcat(p,buf);

        }
    }
    choose(connfd,p);
}

int main(int argc,char **argv)
{
    int i;
    if(argc < 3)
    {
        printf("the argument is error\n");
        return -1;
    }
    int port = atoi(argv[2]);
    const char *ip = argv[1];
    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&server_addr.sin_addr);
    server_addr.sin_port = htons(port);
    
    if((listenFd = socket(PF_INET,SOCK_STREAM,0)) == -1)
    {
        perror("socket error");
        return -1;
    }

    if(bind(listenFd,(struct sockaddr *)&server_addr,sizeof(server_addr)) == -1)
    {
        perror("bind error");
        return -1;
    }

    if(listen(listenFd,5) == -1)
    {
        perror("listen error");
        return -1;
    }

    if(tpool_create(2) != 0) //  init the thread pool 
    {
        return -1;
    }

    int epollfd = epoll_create(5);
    
    struct epoll_event event[MAX_EVENT_NUMBER];
    
    addfd(epollfd,listenFd);  //

    while(1)                     // event loop 
    {
        int number = epoll_wait(epollfd,event,MAX_EVENT_NUMBER,-1);
        if(number < 0)
        {
            break;
        }
        for(i = 0;i < number;i++)
        {
            int sockfd = event[i].data.fd;
            if(sockfd == listenFd)
            {
                struct sockaddr_in client_address;
                socklen_t length = sizeof(client_address);
                int connfd = accept(listenFd,(struct sockaddr *)&client_address,&length);
                addfd(epollfd,connfd);  // main thread  only accept the socket 
            }

            else if(event[i].events & EPOLLIN)
            {
                tpool_add_work(func,(void *)&sockfd); 

                int e_ret = epoll_ctl(epollfd,EPOLL_CTL_DEL,sockfd,NULL); // 删除事件

            }
        }
    } 

    tpool_destroy();  // done 
    close(listenFd);
    return 0;
}
