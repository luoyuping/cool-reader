#include "head.h"
#include "test.h"


int regist(int fd,char *json)
{
    int size = 0;
    char *result = NULL;
    int ret = 0;
    int length = 0;

    string ip = "192.168.1.105";
    string name = "root";
    string mysql = "mysql";
    Database *reg = new Database(ip,name,mysql);
    printf("before connect\n");
    ret = reg->sqlconnect(&result);
    if(ret == ERROR)
    {
         return ERROR;
    }
    printf("after connect\n");
    ret = reg->sqlRegister(json,&result);
    length = strlen(result);
    if(ret == ERROR)
    {
        return ERROR;
    }
    printf("%s\n",result);
    while(BUF_SIZE < length)
    {
        if(0 > send(fd,result+size,BUF_SIZE,0))
        {
            perror("regist send error");
        }

        length -= BUF_SIZE;
        size += BUF_SIZE;
    }

    if(0 != length)
    {
        if(0 > send(fd,result+size,length,0))
        {
            perror("regist send error");
        }
        printf("%s\n",result);
    }

    free(result);
    return OK;
}
