#!/bin/bash -e
DATE=`/bin/date '+%Y%m%d'`
DIR=/tmp/files/
WHICHTYPE="$1"
if [ -z "${WHICHTYPE}" ];
then
        WHICHTYPE="daily"
fi
if [ "$WHICHTYPE" = "weekly" ];
then
        tar -jcvf $DIR/resite_archive_full_$DATE.tar.bz2 /archive/leasing-applications /archive/recolor /archive/uploads
        scp $DIR/resite_archive_full_$DATE.tar.bz2 backup@dsrscbkp01:~/resite/archive/.
        ssh backup@dsrscbkp01 "find /archive/primary/resite/archive/resite_archive_full_* -mtime +5 -exec rm {} \;"
        rm -rf $DIR/*
elif [ "$WHICHTYPE" = "daily" ];
then
        find /archive/uploads -type f -mtime -2 -print0 |xargs -0 tar -pcf $DIR/archive_uploads_incr_$DATE.tar.gz
        find /archive/leasing-applications -type f -mtime -2 -print0 |xargs -0 tar -pcf $DIR/archive_leaseApps_incr_$DATE.tar.gz
        find /archive/recolor -type f -mtime -2 -print0 |xargs -0 tar -pcf $DIR/archive_recolor_incr_$DATE.tar.gz
        tar -jcvf $DIR/resite_archive_incr_$DATE.tar.bz2 $DIR/archive_uploads_incr_$DATE.tar.gz $DIR/archive_leaseApps_incr_$DATE.tar.gz $DIR/archive_recolor_incr_$DATE.tar.gz
        scp $DIR/resite_archive_incr_$DATE.tar.bz2 backup@dsrscbkp01:~/resite/archive/.
        ssh backup@dsrscbkp01 "find /archive/primary/resite/archive/resite_archive_incr_* -mtime +7 -exec rm {} \;"
        rm -rf $DIR/*
fi

