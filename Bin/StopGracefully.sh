#!/bin/bash

source ScriptCommon.sh

send_signal Connector $SIG_STOP_FORCE
send_signal GMConnector $SIG_STOP_FORCE

./Stop.sh GameServer
