#!/bin/bash

for((i=1;i<=100;++i))
do
    s=$[$s+$i]
done

echo $s
