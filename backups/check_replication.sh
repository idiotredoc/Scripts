#!/bin/bash
#this script runs every 15 minutes, checks the status of the replication delay and if necessary, emails the status of the mysql replication to systems@resiteonline.com

USER="repluser"
PW="TnpCm27c"
DB="percona"
HOST="10.93.72.131"
ARG="--check"
CMD="pt-heartbeat -u $USER -p$PW -D $DB -h $HOST $ARG"

RES=`$CMD`
RESULT=`echo "$RES>300.00" | bc`
if [[ $RESULT -eq 1 ]] ;then
	HOST="10.93.72.129"
	CMD="pt-slave-find -u $USER -p$PW -h $HOST"
	$CMD
else
#echo "All is well!"
fi
