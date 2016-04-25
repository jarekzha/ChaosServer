#!/bin/bash

source ScriptCommon.sh

STOP_SIG=$SIG_STOP_GRACEFULLY

#带参数，强制停止
if [ $# -gt 0 ]; then
	STOP_SIG=$SIG_STOP_FORCE
fi

for EXE in ${EXE_LIST}
do
	echo "${EXE}..."
    send_signal $EXE $STOP_SIG
done

./Monitor.sh
