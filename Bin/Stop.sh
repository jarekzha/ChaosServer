#!/bin/bash

source ScriptCommon.sh

if [ $# != 1 ];then
    echo "usage:$0 exeName"
    exit 1
fi

send_signal $1 $SIG_STOP_GRACEFULLY
