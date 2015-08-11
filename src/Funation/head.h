#ifndef HEAD_H
#define HEAD_H 

#define BUF_SIZE 1024
#define ERROR -1
#define OK 0

int login(int fd, char *json);
                    
int regist(int fd, char *json);

int addInfo(int fd,char *json);

#endif



