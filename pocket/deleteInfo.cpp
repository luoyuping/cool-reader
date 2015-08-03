#include "head.h"

int deleteInfo(int fd,const char *json)
{
    Database *dele = new Database();

    int type = 0;
    int length = 0;
    int size = 0;
    string content;
    char *result = NULL;
    Document cJson;
    cJson.Parse(json);
    Value &a = cJson["funContent"];
    a = a["deviceType"];
    content = a.GetString();
    if(!strcmp(content.c_str(),"brower"))
    {
        ret = dele->pcsqlDelete(json,&result);
    }
    else if(!strcmp(content.c_str(),"android"))
    {
        ret = dele->androidsqlDelete(json,&result);
    }
    else 
    {
        printf("not brower or android\n");
        return ERROR;
    }
 
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

