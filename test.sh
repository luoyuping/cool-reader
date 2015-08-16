#!/bin/bash
#对服务器进行并发压力测试

clear 

for (( i = 0; i < 80; i++ )); do
    ./client localhost 12345 ./test_data/test.json
    sleep 0.5
    pstree -p 3558 | wc -l
done
