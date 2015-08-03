#include "head.h"

int sync(int fd,const char *json)
{
    Database syn = new Database();
    int type = 0;
    int length = 0;
    int size = 0;
    int ret;
    string content;
    char *result = NULL;
    Document cJson;
    cJson.Parse(json);
    Value &a = cJson["funContent"];
    a = a["deviceType"];
    content = a.GetString();
    if(!strcmp(content.c_str(),"brower"))
    {
        ret = syn.pcsqlSynchronous(json,&result);
    }
    else if(!strcmp(content.c_str(),"android"))
    {

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
    a = cJson["funType"];
    type = a.GetInt();
    printf("%d\n",type);
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
        if(0 > send(fd,result+size,BUF_SIZE,0))
        {
            perror("sync send error");
        }
    }
    free(result);
    return OK;
}

int main()
{
     FILE *f=fopen("json.txt","rb");fseek(f,0,SEEK_END);long len=ftell(f);fseek(f,0,SEEK_SET);
     char *data=(char*)malloc(len+1);fread(data,1,len,f);fclose(f);
     sync(1,data);
}
