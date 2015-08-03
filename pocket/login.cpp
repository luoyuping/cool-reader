#include "head.h"
#include "Database.h"

int login(int fd,char *json)
{
    string ip = "192.168.1.105";
    string name = "root";
    string mysql = "mysql";
    Database *log = new Database(ip,name,mysql);
    int size = 0;
    char *result = NULL;
    int ret = 0;
    int length = 0;
    ret=log->sqlconnect(&result);
    if(ret == ERROR)
    {
	return ERROR;
    }
    ret = log->sqlLogin(json,&result);
    length = strlen(result);
    printf("%s\n",result);  
    if(ret != OK)
    {
        return ERROR;
    }
    while(BUF_SIZE < length)
    {
        if(0 > send(fd,result+size,BUF_SIZE,0))
        {
            perror("login send error");
        }
        length -= BUF_SIZE;
        size += BUF_SIZE;
    }
    if(0 != length)
    {
        if(0 > send(fd,result+size,length,0))
        {
            perror("login send error");
        }
        printf("%s\n",result);
    }
    free(result);
    return OK;
}


