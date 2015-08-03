#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>


#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/stringbuffer.h"


#include "head.h"
#include "Database.h"

using namespace rapidjson;
//using std::

int login(int fd, char *json)
{
    
    string ip = "192.168.1.105";
    string name = "root";
    string mysql = "mysql";
    Database *log = new Database(ip,name,mysql);

    char *result = NULL;
    int ret = 0;            //标识数据库的
    int length = 0;         //总的数据长度
    //int sendSize =0;        // 已经发送出去的长度

    ret=log->sqlconnect(&result);
    if(ret == ERROR)
    {
	cout << "conect database error" << endl;
	return ERROR;
    }
    ret = log->sqlLogin(json,&result);
    length = strlen(result);
    printf("%s\n",result);  
    if(ret != OK)
    {
        return ERROR;
    }
    
//已经获取到数据了 ，等待将数据发送出去  lenght 是数据长度，size 用来标识每次发送出去的长度
    /*
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
*/
    ret = send(fd,result,length,0);
    if(ret == 0)
    {
        cout << "the client close the socket"<< endl;
    }    
    if (ret == -1 && errno != EAGAIN) {
        cout << "send error,error no is :"<< errno <<endl;
    }     
    
     
    cout << "length: " << length << endl;
    cout << "totally send: " << ret << endl;
    free(result);

    return OK;
}


int regist(int fd, char *json)
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
    /*
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
*/
    ret = send(fd,result,length,0);
    if(ret == 0)
    {
        cout << "the client close the socket"<< endl;
    }    
    if (ret == -1 && errno != EAGAIN) {
        cout << "send error,error no is :"<< errno <<endl;
    }     
    
     
    cout << "length: " << length << endl;
    cout << "totally send: " << ret << endl;

    free(result);
    return OK;
}

// 
const static  char **arg ;

void child(void){
    printf("this is child\n");
    int ret = execl ("/home/luo/server/Funation/goquery","goquery" , arg[0] , arg[1] , NULL) ;
    if (ret < 0)
    { 
        //cout << "execle error" <<endl;
        perror("execle error:" );         
    } 
    exit(0);
}

int addInfo(int fd,char *json)
{
    char ch;
    char *data = NULL;
    int size = 0;
    char *result = NULL;
    int ret = 0;
    int length = 0;
    char fds[8];
    char url[256];
    sprintf(fds,"%d",fd);
    string content,device;
    Document cJson;
    pid_t pid;
    cJson.Parse(json);
    Value &a = cJson["funContent"];
    Value &b = a["url"];
    content = b.GetString();
    b = a["deviceType"];
    device = b.GetString();
    length = content.length();
    const char * argu[2] = {NULL , NULL } ;
    argu[0] = content.c_str() ;
    argu[1] = fds ;
    int ret2 ;
    if((ret2 = pthread_atfork(NULL,NULL,child)) < 0)
    {
        perror("pthread_atfork");
    }

    arg = argu ;

    if (!fork() ) {
    } else {
       wait(NULL) ;
    }

    printf("hello\n");
    sprintf(url,"/tmp/index%d.html",fd);
    ret = open(url,O_RDWR);
    if(ret == ERROR)
    {
        perror("open");   // 
        return ERROR;
    }
    length = lseek(ret,0,SEEK_END);
    lseek(ret,0,SEEK_SET);
    printf("%d\n",length);
    data = (char *)malloc(length);
    if(data == NULL)
    {
        perror("malloc");
        return ERROR;
    }

    read(ret,data,length);

    int i = 0;
    while(data[i] != ':')
    {
        i++;
    }
    int s = i+1;
    while(data[i] != '\n')
    {
        i++;
    }
    int l = i;
    char *urltitle = (char *)malloc(l-s);
    strncpy(urltitle,data+s,l-s);

    //string ip = IP;
    //string name = NAME;
    //string mysql = "";

    string ip = "192.168.1.105";
    string name = "root";
    string mysql = "mysql";

    Database *add = new Database(ip,name,mysql);

    ret = add->sqlconnect(&result);
    if(ret == ERROR)
    {
        cout << "connect database error" << endl;
        return ERROR;
    }
    if(!strcmp(device.c_str(),"brower"))
    {
        ret = add->pcsqlAdditive(json,urltitle,data,&result);
    }
    else if(!strcmp(device.c_str(),"android"))
    {
        ret = add->androidsqlAdditive(json,urltitle,data,&result);
    }
    else
    {
        printf("not brower or android\n");
        return ERROR;
    }
    if(ret == ERROR)
    {
        return ERROR;
    }
    length = strlen(result);
    while(BUF_SIZE < length)
    {
        if(0 > send(fd,result+size,BUF_SIZE,0))
        {
            perror("add send error");
            return ERROR;
        }
        length -= BUF_SIZE;
        size += BUF_SIZE;
    }
    if(0 < length)
    {
        if(0 > send(fd,result+size,BUF_SIZE,0))
        {
            perror("add send error");
            return ERROR;
        }
    }
    //remove(url);
    free(urltitle);
    free(data);
    free(result);
    return OK;
}



