#!/bin/bash 
DATE=`/bin/date '+%Y%m%d'`
DIR=/logs/tmp/files
TMPDIR=$DIR/tmp
INITTIME=$(date +%S)
day=`/bin/date '+%d'`
if [ $day -eq 1 ];
then
        echo "It's the first of the month! Dont run."
        exit
fi
mkdir $TMPDIR
WHICHTYPE="$1"
if [ -z "${WHICHTYPE}" ];
then
        WHICHTYPE="daily"
fi
if [ "$WHICHTYPE" = "weekly" ];
then
	echo "Beginning full backup of uploads, leasing applications, and recolors..."
	sudo tar -xcf $DIR/resite_archive_full_$DATE.tar.gz /archive/leasing-applications /archive/recolor /archive/uploads
        echo "Copying to dsrscbkp01"
	scp $DIR/resite_archive_full_$DATE.tar.gz backup@dsrscbkp01:~/resite/archive/.
	ssh backup@dsrscbkp01 "find /archive/primary/resite/archive/resite_archive_full_* -mtime +30 -exec rm {} \;"
	rm -rf $DIR/*
elif [ "$WHICHTYPE" = "daily" ];
then
	echo "Beginning backup of uploads..."
	sudo find /archive/uploads -type f -mtime -2 -print0 |xargs -r -0 tar -pzcf $TMPDIR/archive_uploads_incr_$DATE.tar.gz
	echo "Beginning backup of leasing apps..."
	sudo find /archive/leasing-applications -type f -mtime -2 -print0 |xargs -r -0 tar -pzcf $TMPDIR/archive_leaseApps_incr_$DATE.tar.gz
	echo "Beginning backup of recolors..."
	sudo find /archive/recolor -type f -mtime -2 -print0 |xargs -r -0 tar -pzcf $TMPDIR/archive_recolor_incr_$DATE.tar.gz
        tar -czf $DIR/resite_archive_incr_$DATE.tar.gz $TMPDIR/*
	echo "Copying to dsrscbkp01"
        scp $DIR/resite_archive_incr_$DATE.tar.gz backup@dsrscbkp01:~/resite/archive/.
        ssh backup@dsrscbkp01 "find /archive/primary/resite/archive/resite_archive_incr_* -mtime +7 -exec rm {} \;"
	rm -rf $DIR/*
fi
ENDTIME=$(date +%S)
TTIME=$(( $INITTIME - $ENDTIME ))
echo "Total runtime: $TTIME seconds"
