#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/stringbuffer.h"


#include "head.h"
#include "Database.h"

#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int regist(int fd, char *json)
{
    int size = 0;
    char *result = NULL;
    int ret = 0;
    int length = 0;
    Database *reg = new Database();

    ret = reg->sqlRegister(json,&result);
    length = strlen(result);

    if(ret != OK)
    {
        return ERROR;
    }

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
    }

    free(result);
    return OK;
}
