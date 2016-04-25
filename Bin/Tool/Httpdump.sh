#!/bin/bash

if [ $# != 1 ]; then
	echo "Usage: $0 [lo|eth0]"
	exit -1
fi

tcpdump -i $1 tcp port 80 -n -X -s 0 -w ./tcp.cap

