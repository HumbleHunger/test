#!/bin/bash

function sum()
{
    s=0
    s=$[$1+$2]
    echo $s
}

read -p "the 1" p1
read -p "the 2" p2
sum $p1 $p2

