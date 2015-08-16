/*
 *使用线程池来处理单个请求  ，IO并发的模型是 模拟proactor ,也就是说，主线程接受好了数据，子线程来做逻辑业务 
 * luoyuping 
 * luohan1124@gmail.com
 */
#include <pthread.h>
#include <unistd.h>
#include <iostream> 
#include "thread_pool.h"

#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/stringbuffer.h"


#include "thread.h"
#include "funMacro.h"

#include "../Funation/head.h" 
// unit test header
#include <fstream>
#include <string>

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
                break;
            }
        case REGISTER:
            {
                cout << "this is register requlest" << endl;
                regist(data->fd,data->json);
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
                addInfo(data->fd,data->json);
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




void* letItfly(void* data)
{
    decodeJson((DataToThread*)data); // 处理逻辑 
    // 延迟 2 秒
    //sleep(2);
    close(((DataToThread*)data)->fd);
    free(data);
    return(0);

}

void throwItTOThread(DataToThread* data)  // 接入epoll 函数
{
    // 讲任务加入线程池的任务队列
    tpool_add_work(letItfly,data);
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
