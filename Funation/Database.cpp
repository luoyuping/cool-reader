//#include "rapidjson/document.h"
//#include "rapidjson/writer.h"
//#include "rapidjson/stringbuffer.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <sstream>
#include <mysql/mysql.h>
#include "Database.h"
#define OK 0
#define ERROR -1

using namespace std;
using namespace rapidjson;


void Database::sqlOk(char ** sqlResqonse)
{
	string temp;
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	writer.StartObject();
	writer.Key("funType");
	writer.Uint(200);
	writer.Key("describe");
	writer.String("OK");
	writer.EndObject();
	temp = buffer.GetString();
	*sqlResqonse = (char *)malloc(temp.length());
	strcpy(*sqlResqonse,temp.c_str());
	return ;
}

void Database::sqlError(char **sqlResqonse)
{	
	string temp;
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	writer.StartObject();
	writer.Key("funType");
	writer.Uint(202);
	writer.Key("describe");
	writer.String("SERVER_ERROR");
	writer.EndObject();
	temp = buffer.GetString();
	*sqlResqonse = (char *)malloc(temp.length());
        strcpy(*sqlResqonse,temp.c_str());
	return ;
}

int Database::sqlconnect(char **sqlResqonse)
{
        pData = mysql_init(NULL);
        if (mysql_real_connect(pData, ip.c_str(), mysqlaccount.c_str(), mysqlpwd.c_str(), "summer_program", 0, NULL, 0))
        //if (mysql_real_connect(pData,"127.0.0.1","root","", "pocket", 0, NULL, 0))
        {
                
                if (mysql_select_db(pData, "pocket") < 0) //选择制定的数据库失败
                {
                        mysql_close(pData);//初始化mysql结构失败
                        sqlError(sqlResqonse);
                        flag = ERROR;
                        //cout << "数据库查找失败，数据库连接关闭" << endl;
                        //cout << "\n已连到上数据库\n" << endl;
                }
                else
                {
                        flag = OK;  //数据库查找正确
			cout << "数据库已连接。\n"<< endl;
                }
        }
        else
        {
                mysql_close(pData);//初始化mysql结构失败
                // cout << "\n数据库连接失败，数据库连接关闭.........\n";
                sqlError(sqlResqonse);
                flag = ERROR;
        }
	return flag;
}

Database::Database(string ipaddr,string  account,string pwd)
{	
	ip=ipaddr;
	mysqlaccount=account;
	mysqlpwd=pwd;
}

Database::~Database()
{
	mysql_close(pData);
}

int Database::sqlLogin(char * json, char ** sqlResqonse)
{	//解析JSON对象得到用户名密码
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	string temp;
	string uemail;
	string upwd; 
	char sql[255];
	string sqlupwd;
	int rt=ERROR;
	Document document;
	document.Parse(json);
	Value& funContent = document["funContent"];
	//assert(a.IsObject());
	Value& userInfo = funContent["userInfo"];
	Value& mailaddr = userInfo["mailAddr"];
	Value& password = userInfo["password"];
	uemail = mailaddr.GetString();
	upwd = password.GetString();
	sprintf(sql, "select * from usertable where uemail='%s'", uemail.c_str());
	cout << sql << endl;
	rt = mysql_real_query(pData, sql, strlen(sql));
	if (rt)
	{
		sqlError(sqlResqonse);
		return rt= ERROR;

	}
	else
	{
		cout << "executed!\n" << endl;
	}
	res = mysql_store_result(pData);
	row = mysql_fetch_row(res);
	if(row==0)
	{
		writer.StartObject();
                writer.Key("funType");
                writer.Uint(201);
                writer.Key("describe");
                writer.String("USER_NOT_EXIST");
                writer.EndObject();
                temp = buffer.GetString();
                *sqlResqonse = (char *)malloc(temp.length());
                strcpy(*sqlResqonse,temp.c_str());
                cout << "Error making query:" << mysql_error(pData) << endl;  //未查找到
                rt = OK;
		return rt;
	}	
	sqlupwd = row[3];
	if (strcmp(sqlupwd.c_str(), upwd.c_str()) == 0)
	{
		flag = 1;  //账户密码正确
		sqlOk(sqlResqonse);
		rt = OK;
	}
	else
	{
		//密码错误
		writer.StartObject();
		writer.Key("funType");
		writer.Uint(203);
		writer.Key("describe");
		writer.String("WRONG_PASSWD");
		writer.EndObject();
		temp = buffer.GetString();
		*sqlResqonse = (char *)malloc(temp.length());
                strcpy(*sqlResqonse,temp.c_str());
		rt = OK;
	}
	mysql_free_result(res);
	return rt;
}

int Database::sqlRegister(char * json, char ** sqlResqonse)
{
	string uname;
	string upwd;
	string uemail;
	string usex;
	string uregister_time ="2015-07-28-15-25";
	char sql[255];
	int rt = ERROR;
	string temp;
	cout<<json<<endl;
	Document document;
	document.Parse(json);
	Value& funContent = document["funContent"];
	//assert(a.IsObject());
	Value& userInfo = funContent["userInfo"];
	Value& mailaddr = userInfo["mailAddr"];
	Value& password = userInfo["password"];
	Value& sex = userInfo["Sex"];
	Value& userName = userInfo["userName"];
	uname = userName.GetString();
	usex = sex.GetString();
	uemail = mailaddr.GetString();
	upwd = password.GetString();
	cout<<uemail<<endl;
	sprintf(sql,"select uemail from usertable where uemail='%s'",uemail.c_str());
	cout<<uemail<<endl;
	rt = mysql_real_query(pData, sql, (unsigned int)strlen(sql));
	res = mysql_store_result(pData);
        row = mysql_fetch_row(res);
        if(row!=0)
	{
		cout<<"用户已存在"<<endl;
		sqlError(sqlResqonse);
                rt = OK;
		return rt;
	}
	sprintf(sql,"insert into usertable(uname,uemail,upwd,usex,uregistertime) values('%s','%s','%s','%s','%s')", uname.c_str(), uemail.c_str(), upwd.c_str(), usex.c_str(), uregister_time.c_str());
	cout<<sql<<endl;
        rt = mysql_real_query(pData, sql, (unsigned int)strlen(sql));
	if (rt) {
		//printf("Register data failure!\n");
		sqlError(sqlResqonse);
		rt = ERROR;
	}
	else {
		printf("Register data success!\n");
		sqlOk(sqlResqonse);
		rt = OK;
	}
	return rt;
}


int Database::pcsqlAdditive(char *json,char *urltitle,char *urldata, char **sqlResqonse)
{
	int rt;
	char *sql=(char *)malloc(strlen(urldata)+255);
	string uemail;
	string url;
	string temp;
	string wurltitle(urltitle);
	string wurldata(urldata);
	StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
	Document document;
	document.Parse(json);
	Value& funContent = document["funContent"];
	Value& mailAddr = funContent["mailAddr"];
	Value& uurl = funContent["url"];
	url=uurl.GetString();
	uemail=mailAddr.GetString();
	cout<<"asdasda"<<endl;
        sprintf(sql, "select * from mapping where url='%s' and uemail='%s' ", url.c_str(),uemail.c_str());
	//cout<<sql<<endl;	
	rt = mysql_real_query(pData, sql, strlen(sql));
	if (rt)	
	{	
		sqlError(sqlResqonse);
		free(sql);
                return rt=ERROR;
	}		
	else
	{
		res = mysql_store_result(pData);
	        row = mysql_fetch_row(res);
       		if(row!=0)
        	{
        	        writer.StartObject();
                	writer.Key("funType");
                	writer.Uint(205);
                	writer.Key("describe");
               	 	writer.String("URL_ALREADY_EXIST");
                	writer.EndObject();
                	temp = buffer.GetString();
                	*sqlResqonse = (char *)malloc(temp.length());
                	strcpy(*sqlResqonse,temp.c_str());
                	rt = OK;
			free(sql);
                	return rt;
        	}
		else
		{	
			sprintf(sql, "insert into mapping(url,uemail,status) values('%s','%s',2)", url.c_str(),uemail.c_str());
			//cout<<sql<<endl;			
			rt = mysql_real_query(pData, sql, strlen(sql));
			if (rt)
                        {
                                sqlError(sqlResqonse);
				free(sql);
                                return rt=ERROR;
                        }
			sprintf(sql, "select * from urltable where url='%s'", url.c_str());
			//cout<<sql<<endl;			
			rt = mysql_real_query(pData, sql, strlen(sql));
		        if (rt)
       			{
                		sqlError(sqlResqonse);
				free(sql);
               			return rt=ERROR;
       			}
       			else
        		{	
				res = mysql_store_result(pData);
        		        row = mysql_fetch_row(res);
				if(row==0)
				{
					//cout<<"urltable        "<<"insert urltable error"<<endl;
					sprintf(sql, "insert into urltable(url,urltitle,urldata,quotetime) values('%s','%s','%s',1)", url.c_str(),urltitle,urldata);
					//cout<<sql<<endl;					
					rt = mysql_real_query(pData, sql, strlen(sql));
					if (rt)
		                        {
                		                sqlError(sqlResqonse);
                               			return rt=ERROR;
                       			}
					else
					{
						writer.StartObject();
				                writer.Key("funType");
                				writer.Uint(OK);
                				writer.Key("content");
                				writer.StartObject();
						writer.Key("url");
						writer.String(url.c_str());
						writer.Key("title");
						writer.String(wurltitle.c_str());
						writer.Key("info");
						writer.String("");
						writer.EndObject();		
               					writer.EndObject();
                				temp = buffer.GetString();
                				*sqlResqonse = (char *)malloc(temp.length());
                				strcpy(*sqlResqonse,temp.c_str());
						rt=OK;
					}
				}
				else
				{
					sprintf(sql, "update urltable set quotetime=quotetime+1  where url='%s'", url.c_str());
					cout<<sql<<endl;		                	
					rt = mysql_real_query(pData, sql, strlen(sql));
					rt = mysql_real_query(pData, sql, strlen(sql));
                		        if (rt)
                        		{
                                		sqlError(sqlResqonse);
						free(sql);
                              		  	return rt=ERROR;
                        		}
					else
					{
						writer.StartObject();
                                                writer.Key("funType");
                                                writer.Uint(OK);
                                                writer.Key("content");
                                                writer.StartObject();
                                                writer.Key("url");
                                                writer.String(url.c_str());
                                                writer.Key("title");
                                                writer.String(wurltitle.c_str());
                                                writer.Key("info");
                                                writer.String("");
                                                writer.EndObject();
                                                writer.EndObject();
                                                temp = buffer.GetString();
                                                *sqlResqonse = (char *)malloc(temp.length());
                                                strcpy(*sqlResqonse,temp.c_str());
                                                rt=OK;						
					}

				}
			}	

		}
	

	}
	free(sql);
	return rt;
}


int Database::androidsqlAdditive(char * json,char *urltitle,char *urldata, char ** sqlResqonse)
{
	int rt;
        char *sql=(char *)malloc(strlen(urldata)+255);
        string uemail;
        string url;
        string temp;
        string wurltitle(urltitle);
        string wurldata(urldata);
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        Document document;
        document.Parse(json);
        Value& funContent = document["funContent"];
        Value& mailAddr = funContent["mailAddr"];
        Value& uurl = funContent["url"];
        url=uurl.GetString();
        uemail=mailAddr.GetString();
	sprintf(sql, "select * from mapping where url='%s' and uemail='%s' ", url.c_str(),uemail.c_str());
        cout << sql << endl;
        rt = mysql_real_query(pData, sql, strlen(sql));
        if(rt)
        {
                sqlError(sqlResqonse);
		free(sql);
                return rt=ERROR;
        }
        else
        {
                res = mysql_store_result(pData);
                row = mysql_fetch_row(res);
                if(row!=0)
                {
                        writer.StartObject();
                        writer.Key("funType");
                        writer.Uint(205);
                        writer.Key("describe");
                        writer.String("URL_ALREADY_EXIST");
                        writer.EndObject();
                        temp = buffer.GetString();
                        *sqlResqonse = (char *)malloc(temp.length());
                        strcpy(*sqlResqonse,temp.c_str());
                        rt = OK;
			free(sql);
                        return rt;
                }
		else
		{
			sprintf(sql, "insert into mapping(url,uemail,status) values('%s','%s',0)", url.c_str(),uemail.c_str());
                        rt = mysql_real_query(pData, sql, strlen(sql));
                        cout<<sql<<endl;
                        if (rt)
                        {
                                sqlError(sqlResqonse);
				free(sql);
                                return rt=ERROR;
                        }
			sprintf(sql, "select * from urltable where url='%s'", url.c_str());
                        cout<<sql<<endl;
                        rt = mysql_real_query(pData, sql, strlen(sql));
                        if (rt)
                        {
                                sqlError(sqlResqonse);
				free(sql);
                                return rt=ERROR;
                        }
                        else
                        {
                                res = mysql_store_result(pData);
                                row = mysql_fetch_row(res);
                                if(row==0)
                                {
                                        sprintf(sql, "insert into urltable(url,urltitle,quotetime,urldata) values('%s','%s',1,'%s')", url.c_str(),urltitle,urldata);
                                        rt = mysql_real_query(pData, sql, strlen(sql));
                                        cout<<sql<<endl;
                                        if (rt)
                                        {
                                                sqlError(sqlResqonse);
						free(sql);
                                                return rt=ERROR;
                                        }
                                        else
                                        {
                                                writer.StartObject();
                                                writer.Key("funType");
                                                writer.Uint(OK);
                                                writer.Key("content");
                                                writer.StartObject();
                                                writer.Key("url");
                                                writer.String(url.c_str());
                                                writer.Key("title");
                                                writer.String(wurltitle.c_str());
                                                writer.Key("info");
                                                writer.String(wurldata.c_str());
                                                writer.EndObject();
                                                writer.EndObject();
                                                temp = buffer.GetString();
                                                *sqlResqonse = (char *)malloc(temp.length());
                                                strcpy(*sqlResqonse,temp.c_str());
                                                rt=OK;
                                        }
                                }
                                else
                                {
                                        sprintf(sql, "update urltable set quotetime=quotetime+1  where url='%s'", url.c_str());
                                        rt = mysql_real_query(pData, sql, strlen(sql));
					cout<<"sad"<<sql<<endl;
                                        if (rt)
                                        {
                                                sqlError(sqlResqonse);
						free(sql);
                                                return rt=ERROR;
                                        }
                                        else
                                        {
                                                writer.StartObject();
                                                writer.Key("funType");
                                                writer.Uint(OK);
						writer.Key("content");
                                                writer.StartObject();
                                                writer.Key("url");
                                                writer.String(url.c_str());
                                                writer.Key("title");
                                                writer.String(wurltitle.c_str());
                                                writer.Key("info");
                                                writer.String(wurldata.c_str());
                                                writer.EndObject();
                                                writer.EndObject();
                                                temp = buffer.GetString();
                                                *sqlResqonse = (char *)malloc(temp.length());
                                                strcpy(*sqlResqonse,temp.c_str());
                                                rt=OK;
                                        }

                                }
                        }

					
		}


	}
	free(sql);
	return rt;
}


int Database::sqlQueryKeywords(char * json, char ** sqlResqonse)
{
	int urlcount=0;
	int rt=0;
	string uemail;
	string ukeywords;
	string url;
	string urltitle;
	string temp;
	char sql[255];
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	Document document;
        document.Parse(json);
	Value& funContent = document["funContent"];
	Value& mailAddr = funContent["mailAddr"];
	Value& keywords = funContent["keywords"];
	uemail = mailAddr.GetString();
	ukeywords = keywords.GetString();
//	sprintf(sql,"select * from mapping where uemail='%s'",uemail.c_str());
	sprintf(sql, "select url from urltable where urltitle like \'%%%s%%\' and url in (select url from mapping where uemail='%s')",ukeywords.c_str(),uemail.c_str());
     	cout<<sql<<endl;
	rt = mysql_real_query(pData, sql, strlen(sql));
        if(rt)
        {
                sqlError(sqlResqonse);
                rt = ERROR;
        }
        else
        {
                res = mysql_store_result(pData);
		row = mysql_fetch_row(res);
               	if(row==0)
		{
			writer.StartObject();
		        writer.Key("funType");
        		writer.Uint(204);
			writer.Key("describe");
			writer.String("URL_NOT_EXIST");
			writer.EndObject();
		        temp = buffer.GetString();
       		 	*sqlResqonse = (char *)malloc(temp.length());
        		strcpy(*sqlResqonse,temp.c_str());
			rt=OK;	
			return rt;
		} 
                
        }
	sprintf(sql, "select url,urltitle from urltable where urltitle like \'%%%s%%\'  and url in (select url from mapping where uemail='%s')",ukeywords.c_str(),uemail.c_str());
	cout<<sql<<endl;
	rt = mysql_real_query(pData, sql, strlen(sql));
	if(rt)
	{
		sqlError(sqlResqonse);
		rt = ERROR;
	}
	res = mysql_store_result(pData);
        while (row = mysql_fetch_row(res))
        {	
		++urlcount;
                url.append(row[0]);
		url.append("&&&&");
		urltitle.append(row[1]);
		urltitle.append("&&&&");
		
	}
	writer.StartObject();
        writer.Key("funType");
        writer.Uint(61);
        writer.Key("content");
        writer.StartObject();
        writer.Key("url");
	writer.String(url.c_str());
	writer.Key("title");
	writer.String(urltitle.c_str());
	writer.Key("urlCount");
	writer.Uint(urlcount);
	writer.EndObject();
        writer.EndObject();
	temp = buffer.GetString();
        *sqlResqonse = (char *)malloc(temp.length());
        strcpy(*sqlResqonse,temp.c_str());
	rt=OK;
	return rt;

}



int Database::pcsqlDelete(char * json, char ** sqlResqonse)
{
        int rt=0;
        string uemail;
        string turl;
        char longurl[1024];
        char *url;
        const char* split="&&&&";
        string temp;
        char sql[255];
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        Document document;
        document.Parse(json);
        Value& funContent = document["funContent"];
        Value& mailAddr = funContent["mailAddr"];
        Value& uurl = funContent["url"];
        uemail = mailAddr.GetString();
        turl=uurl.GetString();
	strcpy(longurl,turl.c_str());
        url=strtok(longurl,split);
        while(url)
        {
		sprintf(sql, "update mapping set status=1 where url='%s' and uemail='%s'",url,uemail.c_str());
		rt = mysql_real_query(pData, sql, strlen(sql));
		if (rt)
		{
				sqlError(sqlResqonse);
				rt = ERROR;
				return rt;
		}
		url=strtok(NULL,split);
	}
	if (rt == OK)
                sqlOk(sqlResqonse);
        return rt;

}


int Database::androidsqlDelete(char * json, char ** sqlResqonse)
{
        int rt=0;
        string uemail;
        string turl;
	char longurl[1024];
	char *url;
	const char* split="&&&&";
        string urltitle;
        string temp;
        char sql[255];
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        Document document;
        document.Parse(json);
        Value& funContent = document["funContent"];
        Value& mailAddr = funContent["mailAddr"];
        Value& uurl = funContent["url"];
        uemail = mailAddr.GetString();
	turl=uurl.GetString();
	strcpy(longurl,turl.c_str());
	url=strtok(longurl,split);
	while(url)
	{
		sprintf(sql, "select quotetime from urltable where url='%s'", url);
		cout<<sql<<endl;
		rt = mysql_real_query(pData, sql, strlen(sql));
		if (rt)
		{
				sqlError(sqlResqonse);
				rt = ERROR;
				return rt;
		}
		MYSQL_RES *res1 = mysql_store_result(pData);
		MYSQL_ROW row1 = mysql_fetch_row(res1);
		string strquotetime = row1[0];
		int quotetime=atoi(strquotetime.c_str());
		if (quotetime > 1)
		{	quotetime = quotetime - 1;
			sprintf(sql, "update urltable set quotetime=%d where url='%s'",quotetime,url);
			cout<<sql<<endl;
			rt = mysql_real_query(pData, sql, strlen(sql));
			if (rt)
			{
				sqlError(sqlResqonse);
				rt = ERROR;
				return rt;
			}
		}
		
		else if (quotetime == 1)
		{
			sprintf(sql, "delete from urltable where url='%s'", url);
			cout<<sql<<endl;
			rt = mysql_real_query(pData, sql, strlen(sql));
			if (rt)
			{	
				sqlError(sqlResqonse);
				rt = ERROR;
				return rt;
			}
			sprintf(sql, "delete from mapping where url='%s'", url);
			cout<<sql<<endl;
			rt = mysql_real_query(pData, sql, strlen(sql));
			if (rt)
			{
				sqlError(sqlResqonse);
				rt = ERROR;
				return rt;
			}
		}

		url=strtok(NULL,split);
	}
	if (rt == OK)
		sqlOk(sqlResqonse);
	return rt;
	
}



int Database::pcsqlSynchronous(char *json, char ** sqlResqonse)
{
	int rt=OK;
	int urlcount=0;
        string uemail;
        string url;
        string urltitle;
        string temp;
        char sql[255];
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        Document document;
        document.Parse(json);
        Value& funContent = document["funContent"];
        Value& mailAddr = funContent["mailAddr"];
        uemail = mailAddr.GetString();
	sprintf(sql, "select url,urltitle from urltable where url in (select url from mapping where uemail='%s')",uemail.c_str());
        cout<<sql<<endl;
        rt = mysql_real_query(pData, sql, strlen(sql));
        if(rt)
        {
                sqlError(sqlResqonse);
                rt = ERROR;
        }
        res = mysql_store_result(pData);
        while (row = mysql_fetch_row(res))
        {
                ++urlcount;
                url.append(row[0]);
                url.append("&&&&");
                urltitle.append(row[1]);
                urltitle.append("&&&&");

        }
	writer.StartObject();
        writer.Key("funType");
        writer.Uint(37);
        writer.Key("content");
        writer.StartObject();
        writer.Key("url");
        writer.String(url.c_str());
        writer.Key("title");
        writer.String(urltitle.c_str());
	writer.Key("info");
	writer.String("");
        writer.Key("urlCount");
        writer.Uint(urlcount);
        writer.EndObject();
        writer.EndObject();
        temp = buffer.GetString();
        *sqlResqonse = (char *)malloc(temp.length());
        strcpy(*sqlResqonse,temp.c_str());
        rt=OK;
        return rt;

}


int Database::androidsqlDeleteSynchronous(char * json, char ** sqlResqonse)
{
	int rt=OK;
        string uemail;
        string url;
        string urltitle;
        string temp;
        char sql[255];
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        Document document;
        document.Parse(json);
        Value& funContent = document["funContent"];
        Value& mailAddr = funContent["mailAddr"];
        uemail = mailAddr.GetString();
	sprintf(sql, "select url from mapping  where uemail='%s' and status=1",uemail.c_str());
        cout<<sql<<endl;
        rt = mysql_real_query(pData, sql, strlen(sql));
        if(rt)
        {
                sqlError(sqlResqonse);
                rt = ERROR;
        }
        res = mysql_store_result(pData);
        while (row = mysql_fetch_row(res))
        {
                url.append(row[0]);
                url.append("&&&&");

        }
	writer.StartObject();
        writer.Key("funType");
        writer.Uint(36);
        writer.Key("content");
        writer.StartObject();
        writer.Key("url");
        writer.String(url.c_str());
        writer.EndObject();
        writer.EndObject();
        temp = buffer.GetString();
        *sqlResqonse = (char *)malloc(temp.length());
        strcpy(*sqlResqonse,temp.c_str());

	sprintf(sql, "select url from mapping  where uemail='%s' and status=1",uemail.c_str());
        cout<<sql<<endl;
        rt = mysql_real_query(pData, sql, strlen(sql));
        if(rt)
        {
                sqlError(sqlResqonse);
                rt = ERROR;
        }
        res = mysql_store_result(pData);
	while(row = mysql_fetch_row(res))
        {	cout<<row<<endl;
		url=row[0];
        	sprintf(sql, "select quotetime from urltable where url='%s'", url.c_str());
                cout<<sql<<endl;
                rt = mysql_real_query(pData, sql, strlen(sql));
                if (rt)
                {
                                sqlError(sqlResqonse);
                                rt = ERROR;
                                return rt;
                }
                MYSQL_RES *res1 = mysql_store_result(pData);
                MYSQL_ROW row1 = mysql_fetch_row(res1);
                string strquotetime = row1[0];
                int quotetime=atoi(strquotetime.c_str());
                if (quotetime > 1)
                {       quotetime = quotetime - 1;
                        sprintf(sql, "update urltable set quotetime=%d where url='%s'",quotetime,url.c_str());
                        cout<<sql<<endl;
                        rt = mysql_real_query(pData, sql, strlen(sql));
                        if (rt)
                        {
                                sqlError(sqlResqonse);
                                rt = ERROR;
                                return rt;
                        }
                }
        	else if (quotetime == 1)
                {
                        sprintf(sql, "delete from urltable where url='%s' and uemail='%s'", url.c_str(),uemail.c_str());
                        cout<<sql<<endl;
                        rt = mysql_real_query(pData, sql, strlen(sql));
                        if (rt)
                        {
                                sqlError(sqlResqonse);
                                rt = ERROR;
                                return rt;
                        }
                }
		sprintf(sql, "delete from mapping where url='%s' and uemail='%s'", url.c_str(),uemail.c_str());
                cout<<sql<<endl;
                rt = mysql_real_query(pData, sql, strlen(sql));
                if (rt)
                {
                         sqlError(sqlResqonse);
                         rt = ERROR;
                         return rt;
                }	
        }
        rt=OK;
        return rt;
}





int Database::androidsqlSynchronous(char * json, char ** sqlResqonse)
{
	int rt=OK;
	int urlcount=0;
        string uemail;
        string url;
        string urltitle;
	string urldata;
        string temp;
        char sql[255];
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        Document document;
        document.Parse(json);
        Value& funContent = document["funContent"];
        Value& mailAddr = funContent["mailAddr"];
        uemail = mailAddr.GetString();
        sprintf(sql, "select url,urltitle,urldata from urltable where url in (select url from mapping where status=2 and uemail='%s')",uemail.c_str());
        cout<<sql<<endl;
        rt = mysql_real_query(pData, sql, strlen(sql));
        if(rt)
        {
                sqlError(sqlResqonse);
                rt = ERROR;
        }
        res = mysql_store_result(pData);
        while (row = mysql_fetch_row(res))
        {
                ++urlcount;
                url.append(row[0]);
                url.append("&&&&");
                urltitle.append(row[1]);
                urltitle.append("&&&&");
		urldata.append(row[2]);
		urldata.append("&&&&");
	}
	writer.StartObject();
        writer.Key("funType");
        writer.Uint(37);
        writer.Key("content");
        writer.StartObject();
        writer.Key("url");
        writer.String(url.c_str());
        writer.Key("title");
        writer.String(urltitle.c_str());
        writer.Key("info");
        writer.String(urldata.c_str());
        writer.Key("urlCount");
        writer.Uint(urlcount);
        writer.EndObject();
        writer.EndObject();
        temp = buffer.GetString();
        *sqlResqonse = (char *)malloc(temp.length());
        strcpy(*sqlResqonse,temp.c_str());
	sprintf(sql, "update mapping set status=0  where status=2 and uemail='%s'",uemail.c_str());
        cout<<sql<<endl;
        rt = mysql_real_query(pData, sql, strlen(sql));
        if(rt)
        {
                sqlError(sqlResqonse);
                rt = ERROR;
        }
	
        rt=OK;
        return rt;
}
