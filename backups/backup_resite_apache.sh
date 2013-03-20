#!/bin/bash -e
DATE=`/bin/date '+%Y%m%d'`
DIR=/logs/tmp/apache
WHICHTYPE="$1"
day=`/bin/date '+%d'`
INITTIME=$(date +%S)
if [ $day -eq 1];
then
        echo "It's the first of the month! Dont run."
        exit
fi
if [ -z "${WHICHTYPE}" ];
then
        WHICHTYPE="daily"
fi
if [ "$WHICHTYPE" = "weekly" ];
then
	sudo tar -cpzf $DIR/resite_web_full_$DATE.tar.gz /var/www --exclude "/var/www/images" --exclude "/var/www/cache" --exclude "/var/www/toBeDeleted" --exclude "/var/www/coreBeta" --exclude "/var/www/coreStaging" --exclude "/var/www/coreTest" --exclude "/var/www/archive"
        scp $DIR/resite_web_full_$DATE.tar.gz backup@dsrscbkp01:~/resite/apache/.
	ssh backup@dsrscbkp01 "find /archive/primary/resite/apache/resite_web_full* -mtime +30 -exec rm {} \;"
        rm -rf $DIR/*
elif [ "$WHICHTYPE" = "daily" ];
then
        sudo find /var/www \( -name cache -o -name images -o -name core -o -name toBeDeleted -o -name archive -o -name coreStaging -o -name coreTest -o -name coreBeta -\) -prune -o -mtime -2 -print0  |xargs -0 sudo tar -pzcf $DIR/resite_web_incr_$DATE.tar.gz
        scp $DIR/resite_web_incr_$DATE.tar.gz backup@dsrscbkp01:~/resite/apache/.
        ssh backup@dsrscbkp01 "find /archive/primary/resite/apache/resite_web_incr* -mtime +7 -exec rm {} \;"
        rm -rf $DIR/*
fi
ENDTIME=$(date +%S)
TTIME=$(( $INITTIME - $ENDTIME ))
echo "Total runtime: $TTIME seconds"

