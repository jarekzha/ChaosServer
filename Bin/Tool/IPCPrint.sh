#!/bin/bash

source ../ScriptCommon.sh

for EXE in ${EXE_LIST}
do
	send_signal ${EXE} $SIG_IPC_PRINT
done
