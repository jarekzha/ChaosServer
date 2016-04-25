#!/bin/bash

source ../ScriptCommon.sh

for EXE in ${EXE_LIST}
do
	send_signal ${EXE} $SIG_OBJECT_COUNTER_PRINT
done
