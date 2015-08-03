#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <sstream>
#include <mysql/mysql.h>
#define OK 0
#define ERROR -1

using namespace std;
using namespace rapidjson;



class Database
{
private:
        MYSQL*  pData;
        MYSQL_RES *res;
        MYSQL_ROW row;
        int flag;
        string ip;
        string mysqlaccount;
        string mysqlpwd;
public:
        Database(string ip,string mysqlaccount,string mysqlpwd);//构造函数
        int sqlconnect(char **sqlResqonse);//建立连接
        int sqlLogin(char * json, char ** sqlResqonse);//登陆
        int sqlRegister(char * json, char ** sqlResqonse);//注册
        int pcsqlSynchronous(char *json, char ** sqlResqonse);//pc同步
        int androidsqlSynchronous(char * json, char ** sqlResqonse);//android未缓存同步
        int androidsqlDeleteSynchronous(char * json, char ** sqlResqonse);//android未删除同步
        int pcsqlDelete(char * json, char ** sqlResqonse);//pc删除
        int androidsqlDelete(char * json, char ** sqlResqonse);//android删除
        int pcsqlAdditive(char *json,char *urltitle,char *urldata, char **sqlResqonse);//pc添加
        int androidsqlAdditive(char * json,char *urltitle,char *urldata,char ** sqlResqonse);//android添加
        int sqlQueryKeywords(char * json, char ** sqlResqonse);//查询
        void sqlError(char ** sqlResqonse);//错误返回json
        void sqlOk(char ** sqlResqonse);//正确返回json
        ~Database();//关闭数据库连接
};

