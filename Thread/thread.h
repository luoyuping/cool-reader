#ifndef THREAD_H
#define THREAD_H 
/*
 *注意有两个动态申请的对象 提醒后面的对接的人注意释放两个对象
 *buf 是动态申请的 
 * buf 放在 DataToThread 类型的 data中
 * 所以先释放buf 后释放 data
 */

typedef struct DataToThread
{
    int fd;
    char* json;
}DataToThread;

void throwItTOThread(DataToThread* data);

#endif

