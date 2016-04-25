#!/bin/bash

./StopAll.sh 1

sleep 1
./RemoveIPC.sh

./StartAll.sh

