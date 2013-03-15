#!/bin/bash -e
DATE=`/bin/date '+%Y%m%d'`
DIR=/tmp/apache
WHICHTYPE="$1"
if [ -z "${WHICHTYPE}" ];
then
        WHICHTYPE="daily"
fi
if [ "$WHICHTYPE}" -eq "daily" ];
then
	/bin/tar -cpzf $DIR/clientSites-$date.tar.gz /var/www --exclude "/var/www/desi" --exclude "/var/www/images" --exclude "/var/www/cache" --exclude "/var/www/toBeDeleted" --exclude "/var/www/coreBeta" --exclude "/var/www/coreStaging" --exclude "/var/www/coreTest" --exclude "/var/www/archive"
        scp $DIR/clientSites-$date.tar.gz backup@dsrscbkp01:~/resite/archive/.
	rm -rf $DIR/*
fi
