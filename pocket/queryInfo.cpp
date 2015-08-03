#include "head.h"

int queryInfo(int fd,const char *json)
{
    Database *query = new Database();
    int type = 0;
    int length = 0;
    int size = 0;
    string content;
    char *result = NULL;
    
    ret = query->sqlQueryKeyword(json,&result);

    if(ret != OK)
    {
        return ERROR;
    }
    length = strlen(result);
    while(BUF_SIZE < length)
    {
        if(0 > send(fd,result+size,BUF_SIZE,0))
        {
            perror("sync send error");
        }
        length -= BUF_SIZE;
        size += BUF_SIZE;
    }
    if(0 != length)
    {
        if(0 > send(fd,result+size,length,0))
        {
            perror("sync send error");
        }
    }
    free(result);
    return OK;
}

