#ifndef SOCKET_INIT_H
#define SOCKET_INIT_H 
int Server_init_tcp(char* argv[],int listenArrayLength);
int Get_socket_for_client(int listen_socket);
int Client_init_tcp(char* argv[]);

#endif  //SOCKET_INIT_H



