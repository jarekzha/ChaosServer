#!/bin/bash

EXE_LIST="Connector DBProxy GameServer"

# 输出ps首行
ps aux | grep 'USER' | grep -v grep

for EXE in ${EXE_LIST}
do
    ps aux | grep ${EXE} | grep -v grep
done
