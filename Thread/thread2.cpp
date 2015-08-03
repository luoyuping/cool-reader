/*
 *epoll.cpp 中将数据接收到后（数据存贮 是动态申请的）,通过将结构体指针 传入 throwItToThread
 * 在throwItToThread  中 进行 并发 操作 并发控制在该函数中进行  
 * 每个线程的线程函数是letItFly()
 *
 */
#include <pthread.h>
#include <unistd.h>
#include <iostream> 

#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/stringbuffer.h"


#include "thread.h"
#include "funMacro.h"

#include "../Funation/head.h" 
// unit test header
#include <fstream>
#include <string>
#include <string> 
#include <fstream>

#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

using std::ifstream;
using std::streamsize;
using std::string;
using std::ofstream; 
//





using rapidjson::Document;
using rapidjson::StringBuffer;
using rapidjson::Value;
using rapidjson::Writer;

using std::cout;
using std::endl;




// 主线程将数据 json 包 传入到子线程 子线程解析 


void distinguishFun(int funType,DataToThread* data)
{
    switch(funType)
    {
        case LOGIN:
            {
                cout << "this is login request" << endl;
                login(data->fd,data->json);
                /*
                char buffer[100]= "hello";                 
                int ret = send(data->fd,buffer,6,0);
                if (ret < 0)
                {
                    if(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
                    {
                        cout << "the link is normal,but can not send data" << endl;
                    }
                }
                else if(ret == 0)
                {
                    cout << "对方关闭套接字" << endl;
                }
                else 
                {
                    cout << "success send data to client" << endl;
                }
                */

                break;
            }
        case REGISTER:
            {
                cout << "this is register requlest" << endl;
                //regist(data->fd,data->json);
                break;
            }
        case UPDATE:
            {
                cout << "this is datauptate request" << endl;
                break;
            }
        case ADD:
            {
                cout << "this is data add  request" << endl;
                break;
            }
        case DELETE:
            {
                cout << "this is delete request" << endl;
                break;
            }
        case QUERY:
            {
                cout << "this is query request" << endl;
                break;
            }

    } 
}


void decodeJson( DataToThread* data)
{
   // 1. 把JSON解析至DOM。
    //const char* json = "{\"funType\":20,\"funContent\":{\"userInfo\":{\"userName\":\"luoyuping\",\"password\":\"123456\",\"email\":\"120953480@qq.com\",\"Sex\":\"male\"}}}";
    //const char* json = Json;

    cout << data->json << endl;

    
    Document d;
    d.Parse(data->json);
     //2. 利用DOM作出修改。
    Value& s = d["funType"];  // 获取到一个包装类 
    
    int funType =   s.GetInt();
    cout << "the fun type is "<< funType << endl;

      //下一步进行处理
     
    distinguishFun(funType,data);
    //cout << Json <<endl;
    // 将数据接受下来，写入文件，对比区别
    //cout << json << endl;
    //ofstream out;
    //out.open("get.json",ofstream::app);
    //out << json;
    //out.close();
}




void letItfly(DataToThread* data)
{
    decodeJson((DataToThread*)data);
    //pthread_exit(0);

}

void throwItTOThread(DataToThread* data)
{
    // 这函数还是在主线程里执行滴～
    // 对子线程的所有操作都可以放在这 
    //先开一个线程帮你处理请求

    //pthread_t id; 
    //pthread_create(&id,NULL,&letItfly,(void*)data) ;
    //pthread_join(id,0);
    //cout << "pthread die"  <<endl;
    letItfly(data);
    close(data->fd);
    free(data);
}

/*
 * unit test
 */

/*
int main(int argc, char *argv[])
{
    ifstream fin(argv[1]);
    //char buf[1024];
    char buffer[4096];
    memset(buffer,4096,0);

    streamsize   n =0;
    fin.get(buffer,4096);
    n= fin.gcount()+1;
    cout << n <<endl;
    string temp(buffer,n);
    cout << temp << endl;
    
    decodeJson(temp.c_str());
    *
    */
/*
    string temp("{\"funType\":20,\"funContent\":{\"userInfo\":{\"userName\":\"luoyuping\",\"password\":\"123456\",\"email\":\"120953480@qq.com\",\"Sex\":\"male\"}}}");
    decodeJson(temp.c_str());

     

    return 0;
}
*/
