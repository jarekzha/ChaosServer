#!/bin/sh

USER=`whoami`
SHMID=`ipcs -m | grep $USER | awk '{if($2 ~ /^[0-9]+$/) printf("%d\n", $2)}'`

for id in $SHMID; do
    ipcrm -m $id
done

ipcs -m | grep $USER

