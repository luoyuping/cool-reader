# 项目的 Makefile 示例，对于此结构代码，该 Makefile 均通用。
#
# ------------------------------------------------------------------------

# 可以类似这样执行 shell 脚本，find 命令的结果作为 C_SOURCES 变量的内容
# patsubst 貌似是内置函数，它处理所有在 C_SOURCES 字列中的字（一列文件名）
# 如果它的 结尾是 '.c'，就用 '.o' 把 '.c' 取代
C_SOURCES = $(shell find ./src -name "*.cpp")

C_OBJECTS = $(patsubst %.cpp, %.o, $(C_SOURCES))

CC = clang 
LK = clang++

#C_FLAGS = -D FTP_DEBUG -c -Wall -Iinclude -g
C_FLAGS = -c -Wall  -g 
 
LINK_FLAGS = -lpthread  -lmysqlclient
PROGRAM = server

all: $(C_OBJECTS) 
	@echo 链接 ...
	$(LK) $(C_OBJECTS) $(LINK_FLAGS) -o $(PROGRAM)

# The automatic variable `$<' is just the first prerequisite
# $@表示规则的当前目标文件名
.c.o:
	@echo 编译代码文件 $< ...
	$(CC) $(C_FLAGS) $< -o $@
	#clang $(C_FLAGS) $< -o $@
.PHONY:clean

clean:
	@echo 清理临时文件
	#$(RM) $(C_OBJECTS) $(PROGRAM)
	
	$(RM) $(C_OBJECTS)








