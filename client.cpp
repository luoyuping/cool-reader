#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h> 
#include <cstdlib>
#include <assert.h>
#include <string.h>
#include <iostream>
#include <errno.h> 

#include <iostream> 
#include <fstream> 
#include <string>

#define BUFFSIZE 512
using namespace std;


int main (int argc,char* argv[])
{
   //main ss
   if(argc <= 2)
   {
       cout << "usage : ip + port" << endl;
       return 1;
   }
   const char* ip = argv[1];
   int port = atoi(argv[2]);
   /*
    * init socket addr
    */

   struct sockaddr_in server;
   memset(&server,0,sizeof(struct sockaddr_in));

   server.sin_family = AF_INET;

   inet_pton(AF_INET,ip,&server.sin_addr);

   server.sin_port = htons(port);

   int sock = socket(PF_INET,SOCK_STREAM,0);

   char send_buffer[BUFFSIZE] ;
   assert(sock >= 0);

   if((connect(sock,(struct sockaddr*)&server,sizeof(server))) != -1)
   {
       memset(send_buffer,BUFFSIZE,0);
       int jsonLength =0;
       ifstream fin(argv[3]);

       fin.get(send_buffer,4096);
       jsonLength = fin.gcount() ;
       cout << send_buffer << endl;
       cout << jsonLength <<endl;
          

       send(sock,send_buffer,jsonLength,0);
   }
   
   memset(send_buffer,0,BUFFSIZE);
   

   // 循环接收 
    string recv_buf;
    while(1)
    { 
        int ret = recv(sock,send_buffer,BUFFSIZE,0);
        
        if(ret == 0)
        {
            cout << "对方关闭套接字"<< endl;
            break;
        }

        else if (ret < 0) {  // 失败 
            if (errno == EAGAIN) {    // 对方主动关闭套接字,接收超时 
                cout << "超时" << endl;
                continue;
            }  
            else
            {
                cout << " socket 异常" << endl;
                break;
            }
        } 

        else
        {
            cout<< send_buffer << endl;
            string temp(send_buffer);
            recv_buf += temp;  // 追加
            continue;  // 接收下次
        }
    } 
    cout << "totally get:" << recv_buf.length() << endl;

   close(sock);
   return 0;
}


















































