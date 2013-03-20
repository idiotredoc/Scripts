#!/bin/bash
DIR=/archive/primary/resite/
DATESTAMP=$(date +%Y%m%d) 
MOUNT=/mnt/external_backup_hdd
LOCALDIR=$MOUNT/$DATESTAMP
HOST='dsrscbkp01'
echo "Checking if HDD is mounted..."
if [ mount | grep "on ${MOUNT} type" > /dev/null ]; then  
	echo "Creating backup directory..."
	mkdir $LOCALDIR
	echo "Finding files..."
	LIST=`ssh backup@$HOST "find $DIR/ -name files -prune -o print | grep $DATESTAMP"`
	echo "Copying files..."
	for f in $LIST
	do
		scp backup@$HOST:$f $LOCALDIR/.
	done
	echo "Checking if Second HDD is mounted"
	if [ -b /dev/sdc1 ]; then
		echo "Writing copy to second HDD..."
		dd if=/dev/sdb1 of=/dev/sdc1
	else
		echo "/dev/sdc1 is not mounted!"
		exit
	fi
else
	echo "$MOUNT is not mounted!"
	exit
fi
