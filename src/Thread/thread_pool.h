#ifndef THREAD_POOL_H
#define THREAD_POOL_H 
#include <pthread.h>


typedef struct tpool_work  // job list 
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

int tpool_create(int max_thr_num)  ;

void tpool_destroy() ;

int tpool_add_work(void *(*routine)(void *),void *arg) ;  // 被加入任务队列的函数最好是线程安全的 

#endif


