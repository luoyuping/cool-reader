/*
 * 读取mysql 配置文件的程序片段
 * luoyuping
 * luohan1124@gmail.com
 * 文件的位置是 ~/server_improve/server_config/mysql.json
 */
#include <vector> 
#include <string>
#include <fstream>
#include <exception> 
#include "read_conf.h"
#include <iostream>

#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/stringbuffer.h"

using std::string;
using std::ifstream;
using std::vector;
using std::exception;
using std::cout ;
using std::endl;
using rapidjson::Document;
using rapidjson::StringBuffer;
using rapidjson::Value;
using rapidjson::Writer;



//readMysqlConfig::readMysqlConfig(string filename)//:filename(filename)
//{
    //this->filename = filename;
//} 

void readMysqlConfig::dealConfigFile()
{ 
    ifstream* file  = new ifstream();
    try{
    file->open(filename.c_str());
    }
    catch(exception e)
    {
        cout << "open config file failed" << endl;
        exit(-1);                
    }


    string content;
    string temp;
    while (*file >> temp ) { 
        content+= temp; 
    }  
    //cout << content << endl;
    Document d;
    d.Parse(content.c_str());
    //ip = d["ip"];
    //userName = d["username"];
    //this->passwd  = d["password"];
    Value &s = d["username"];
    username = s.GetString();
    s= d["password"];
    passwd = s.GetString();
    s= d["ip"];
    ip = s.GetString(); 

   // test 
   //cout << ip << endl << username << endl << passwd << endl; 
    
} 

string readMysqlConfig::getIP()
{
    //cout << ip << endl; 
    return ip; 
} 

string readMysqlConfig::getPasswd()
{ 
    //cout << passwd << endl; 
    return(passwd);  
} 

string readMysqlConfig::getUsername()
{ 
    //cout << username << endl; 
    return(username);  
} 

void readMysqlConfig::debug()
{
    dealConfigFile();
}



// class test
/*
int main(int argc, char *argv[])
{
    
    readMysqlConfig test("/home/luo/server_improve/server_config/mysql.json");
    //test.debug();
    test.getIP();
    test.getPasswd();
    test.getUsername();
    
    
    return 0;
}
*/













