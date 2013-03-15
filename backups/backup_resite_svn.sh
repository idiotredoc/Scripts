#!/bin/bash -e
screen
date=`/bin/date '+%Y%m%d'`
DIR=/tmp/svn
WHICHTYPE="$1"
if [ -z "$WHICHTYPE" ];
then
        WHICHTYPE="weekly"
fi
if [ "$WHICHTYPE" = "weekly" ];
then
        svn export -q --force http://svn.dev.residentsource.com/svn/rit/trunk/rit /tmp/svn/rit
        svn export -q --force http://svn.dev.residentsource.com/svn/rit/trunk/clientdev /tmp/svn/clientdev
        svn export -q --force http://svn.dev.residentsource.com/svn/rit/projects/core/trunk  /tmp/svn/core-trunk
	tar cf $DIR/resitesvn-full-$date.tar /tmp/svn
	gzip $DIR/resitesvn-full-$date.tar.gz $DIR/resitesvn-full-$date.tar
	scp $DIR/resitesvn-full-$date.tar.gz backup@dsrscbkp01:~/resite/svn/.
	rm -rf /tmp/svn/*

elif [ "$WHICHTYPE" = "daily" ];
then
	yesterday=`/bin/date --date="yesterday" "+%Y-%m-%d"`
	today=`/bin/date "+%Y-%m-%d"`

	svn log  http://svn.dev.residentsource.com/svn/rit/trunk -qv -r {$yesterday}:{$today}|/bin/grep trunk|/bin/cut -d" " -f 5 >/tmp/svn/svnchangelog

	cat /tmp/svn/svnchangelog | while read LINE; do
    	filename=`/bin/echo $LINE | /bin/awk -F/ '{ print $NF }'`
    	fpath=`/bin/echo $LINE | /bin/awk 'BEGIN{FS=OFS="/"}{$NF=""; NF--; print}'`
   	npath=`/bin/echo $fpath | /bin/cut  -c2-`
    	rpath=$DIR/$npath
    	finalpath=$rpath/$filename

    	mkdir -p $rpath
    	svn -q export --force http://svn.dev.residentsource.com/svn/rit/$LINE $finalpath
done
	/bin/tar cf $DIR/resitesvn-incr-$date.tar /tmp/svn
	gzip $DIR/resitesvn-incr-$date.tar.gz $DIR/resitesvn-incr-$date.tar
        scp $DIR/resitesvn-incr-$date.tar.gz backup@dsrscbkp01:~/resite/svn/.
	/bin/rm -rf /tmp/svn/*
	        
fi
screen -X quit
