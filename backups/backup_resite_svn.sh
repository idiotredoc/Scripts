#!/bin/bash 
day=`/bin/date '+%d'`
date=`/bin/date '+%Y%m%d'`
DIR=/logs/tmp/svn
TMPDIR=$DIR/tmp
WHICHTYPE="$1"
INITTIME=$(date +%S)

if [ $day -eq 1 ]; 
then
	echo "It's the first of the month! Dont run."
	exit
fi
mkdir $TMPDIR $TMPDIR/rit $TMPDIR/clientdev $TMPDIR/core-trunk
if [ -z "$WHICHTYPE" ];
then
        WHICHTYPE="weekly"
fi
if [ "$WHICHTYPE" = "weekly" ];
then
	echo "Exporting rit svn..."
        svn export -q --force http://svn.dev.residentsource.com/svn/rit/trunk/rit $TMPDIR/rit
	echo "Exporting clientdev svn..."
        svn export -q --force http://svn.dev.residentsource.com/svn/rit/trunk/clientdev $TMPDIR/clientdev
	echo "Exporting core svn..."
        svn export -q --force http://svn.dev.residentsource.com/svn/rit/projects/core/trunk  $TMPDIR/core-trunk
	tar -zcf $DIR/resite_svn_full_$date.tar.gz $TMPDIR/*
        echo "Copying to dsrscbkp01"
	scp $DIR/resite_svn_full_$date.tar.gz backup@dsrscbkp01:~/resite/svn/.
        ssh backup@dsrscbkp01 "find /archive/primary/resite/svn/resite_svn_full* -mtime +30 -exec rm {} \;"
        rm -rf $TMPDIR
	rm -rf $DIR/*
	

elif [ "$WHICHTYPE" = "daily" ];
then
	yesterday=`/bin/date --date="yesterday" "+%Y-%m-%d"`
	today=`/bin/date "+%Y-%m-%d"`

	svn log  http://svn.dev.residentsource.com/svn/rit/trunk -qv -r {$yesterday}:{$today}|/bin/grep trunk|/bin/cut -d" " -f 4,5 >$TMPDIR/svnchangelog

	cat $TMPDIR/svnchangelog | while read LINE; do
	if [ `/bin/echo $LINE | /bin/awk {'print $1'}` = "D"];
	then
		echo "Skipping `echo $LINE | awk {'print $2'}`, scheduled for deletion"
		continue
	else
	fi
    	filename=`/bin/echo $LINE | /bin/awk -F/ '{ print $NF }'`
    	fpath=`/bin/echo $LINE | /bin/awk 'BEGIN{FS=OFS="/"}{$NF=""; NF--; print}'`
   	npath=`/bin/echo $fpath | /bin/cut  -c2-`
    	rpath=$TMPDIR/$npath
    	finalpath=$rpath/$filename

    	mkdir -p $rpath
    	svn -q export --force http://svn.dev.residentsource.com/svn/rit/$LINE $finalpath
done
	tar -zcf $DIR/resite_svn_incr_$date.tar.gz $TMPDIR/*
	echo "Copying to dsrscbkp01"
        scp $DIR/resite_svn_incr_$date.tar.gz backup@dsrscbkp01:~/resite/svn/.
        ssh backup@dsrscbkp01 "find /archive/primary/resite/svn/resite_svn_incr* -mtime +7 -exec rm {} \;"
	/bin/rm -rf $TMPDIR
	/bin/rm -rf $DIR/*
fi
ENDTIME=$(date +%S)
TTIME=$(( $ENDTIME - $INITTIME ))
echo "Total runtime: $TTIME seconds"

