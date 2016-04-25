#!/bin/sh

PROJECT_DIR=$(cd `dirname $0`; pwd)

echo "=============================="
echo "| Project Dir: ${PROJECT_DIR} "
echo "=============================="

#######
echo "========= make clean ========="
make -f ${PROJECT_DIR}/Src/Makefile clean

########
echo "========= remove log ========="
find ${PROJECT_DIR}/Bin/Log/ -name "*.log" | grep -v "analysis" | xargs rm -rf
