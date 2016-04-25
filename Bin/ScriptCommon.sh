
EXE_LIST="Connector DBProxy GameServer GMConnector"

SIG_STOP_FORCE=9
SIG_STOP_GRACEFULLY=10
SIG_IPC_PRINT=34
SIG_OBJECT_COUNTER_PRINT=35
SIG_RELOAD_CONFIG=36

#$1->procss name 
#$2->signal
function send_signal()
{
	echo "Send signal $2 to $1..."
	ps aux | grep $1 | grep -v grep | awk -v sig=$2 '{printf("kill -s %s %d\n"), sig, $2}' | sh
}



