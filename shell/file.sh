#!/bin/bash
home="/home/liuyuji/test"
f="*"
for file in "${home}/shell/*"
do
    if test -f $file
    then
        echo $file 是文件
    else
        echo $file 是目录
    fi
done
