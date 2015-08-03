all: fun 

#fun:server.o socket_init.o epoll.o thread.o login.o regist.o 
#	clang++ server.o socket_init.o epoll.o thread.o  login.o regist.o  -o fun -lpthread -lmysqlclient

fun:server.o socket_init.o epoll.o thread.o fun.o Database.o
	clang++ server.o socket_init.o epoll.o thread.o  fun.o Database.o  -o fun -lpthread -lmysqlclient -g
server.o: server.cpp 
	clang -c server.cpp -g

socket_init.o: IO/socket_init.cpp
	clang -c IO/socket_init.cpp -g

epoll.o:IO/epoll.cpp 
	clang -c IO/epoll.cpp -g

thread.o:Thread/thread.cpp
	clang -c Thread/thread.cpp -g

fun.o:Funation/fun.cpp
	clang -c Funation/fun.cpp -g

Database.o:Funation/Database.cpp
	clang -c Funation/Database.cpp -g 

#login.o:Funation/login.cpp
#	clang -c Funation/login.cpp 

#regist.o:Funation/regist.cpp
#	clang -c Funation/regist.cpp 


clean:
	rm ./*.o
