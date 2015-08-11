#!/bin/bash
#对服务器进行并发压力测试

clear 

for (( i = 0; i < 80; i++ )); do
    ./client 192.168.1.113 12345 test.json
    sleep 0.5
    pstree -p 3558 | wc -l
done
