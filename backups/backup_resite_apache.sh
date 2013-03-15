#!/bin/bash -e
DATE=`/bin/date '+%Y%m%d'`
DIR=/files/backups/tmp/
WHICHTYPE="$1"
if [ -z "${WHICHTYPE}" ];
then
        WHICHTYPE="daily"
fi
if [ "$WHICHTYPE" = "weekly" ];
then
	tar -cpzf $DIR/clientSites-full-$date.tar.gz /var/www --exclude "/var/www/images" --exclude "/var/www/cache" --exclude "/var/www/toBeDeleted" --exclude "/var/www/coreBeta" --exclude "/var/www/coreStaging" --exclude "/var/www/coreTest" --exclude "/var/www/archive"
        scp $DIR/clientSites-$date.tar.gz backup@dsrscbkp01:~/resite/archive/apache/.
	ssh backup@dsrscbkp01 "find /archive/primary/resite/apache/clientSites-full* -mtime +5 -exec rm {} \;"
        rm -rf $DIR/*
elif [ "$WHICHTYPE" = "daily" ];
then
        find /var/www \( -name cache -o -name images -o -name core -o -name toBeDeleted -o -name archive -o -name coreStaging -o -name coreTest -o -name coreBeta -\) -prune -o -mtime -2 -print0  |xargs -0 tar -pcf $DIR/clientSites-incr-$DATE.tar.gz
        scp $DIR/clientSites-incr-$DATE.tar.bz2 backup@dsrscbkp01:~/resite/archive/apache/.
        ssh backup@dsrscbkp01 "find /archive/primary/resite/apache/clientSites-incr-* -mtime +7 -exec rm {} \;"
        rm -rf $DIR/*
fi


