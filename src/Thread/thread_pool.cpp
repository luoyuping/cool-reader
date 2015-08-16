#include <pthread.h>
#include <cstdlib>
#include "thread_pool.h"  


static tpool_t *tpool = NULL; // 线程池管理数据结构 

pthread_attr_t attr;

static void *thread_routine(void *arg)
{
    tpool_work_t *work;
    while(1)
    {
        pthread_mutex_lock(&tpool->queue_lock);     // 从任务队列里取走任务

        while(tpool->queue_head == NULL && !tpool->shutdown) // 没有任何任务时 ，但没有收到线程退出的信号 
        {
            pthread_cond_wait(&tpool->queue_ready,&tpool->queue_lock);//避免 线程 阻塞 不断的 询问 ，所以使用 条件变量 
        }

        if(tpool->shutdown) // 排除线程要销毁
        {
            pthread_mutex_unlock(&tpool->queue_lock); 

            pthread_exit(NULL);
        }

        // 新的任务

        work = tpool->queue_head;

        tpool->queue_head = tpool->queue_head->next;  // 把任务取走  ，任务指针后移 

        pthread_mutex_unlock(&tpool->queue_lock);

        work->routine(work->arg);     // 调用任务函数 
        free(work);             // 释放节点 避免内存泄露 

    }
    return NULL;
}


int tpool_create(int max_thr_num)  //
{
    int i;

    tpool =(tpool_t *)calloc(1,sizeof(tpool_t));
    if(!tpool)
    {
        exit(1);
    }
    tpool->max_thr_num = max_thr_num;
    tpool->shutdown = 0;
    tpool->queue_head = NULL; 

    pthread_mutex_init(&tpool->queue_lock,NULL);
    pthread_cond_init(&tpool->queue_ready,NULL);
    pthread_attr_init(&attr);

    int ret = pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    
    if(ret == 0) // set successful
    { 
        tpool->thr_id = (pthread_t *)calloc(max_thr_num,sizeof(pthread_t));
        for(i = 0;i < max_thr_num;++i)
        {
            pthread_create(&tpool->thr_id[i],&attr,thread_routine,NULL);  // 新创建的线程的属性是 以分离状态启动线程 
        }
    } 
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
    pthread_attr_destroy(&attr);
    free(tpool);
}

int tpool_add_work(void *(*routine)(void *),void *arg)  // 被加入任务队列的函数最好是线程安全的 
{
    tpool_work_t *work, *member;  

    work = (tpool_work_t *)malloc(sizeof(tpool_work_t));  // 函数链表

    work->routine = routine;  // create a node(type is tpool_work_t) 
    work->arg = arg;
    work->next = NULL;
    pthread_attr_t attr;

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

    //pthread_cond_signal(&tpool->queue_ready); //inform the work thread to work
 
    pthread_mutex_unlock(&tpool->queue_lock);  

    pthread_cond_signal(&tpool->queue_ready); //inform the work thread to work
    return 0;
}

