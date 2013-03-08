#!/bin/sh

date=`/bin/date '+%Y%m%d'`


/usr/bin/find /archive/uploads -type f -mtime -2 -print0 |xargs -0 tar pcf /files/backups/archive-uploads-incr-$date.tar.gz

/usr/bin/find /archive/leasing-applications -type f -mtime -2 -print0 |xargs -0 tar pcf /files/backups/archive-leaseApps-incr-$date.tar.gz

/usr/bin/find /archive/recolor -type f -mtime -2 -print0 |xargs -0 tar pcf /files/backups/archive-recolor-incr-$date.tar.gz

yesterday=`/bin/date --date="yesterday" "+%Y-%m-%d"`
today=`/bin/date "+%Y-%m-%d"`

/usr/local/bin/svn log  http://svn.dev.residentsource.com/svn/rit/trunk -qv -r {$yesterday}:{$today}|/bin/grep trunk|/bin/cut -d" " -f 5 >/files/backups/svnchangelog

cat /files/backups/svnchangelog | while read LINE; do
    filename=`/bin/echo $LINE | /bin/awk -F/ '{ print $NF }'`
    fpath=`/bin/echo $LINE | /bin/awk 'BEGIN{FS=OFS="/"}{$NF=""; NF--; print}'`
    npath=`/bin/echo $fpath | /bin/cut  -c2-`
    rpath=/files/backups/svn/$npath
    finalpath=$rpath/$filename

    mkdir -p $rpath
    /usr/local/bin/svn -q export http://svn.dev.residentsource.com/svn/rit/$LINE $finalpath
done

/bin/tar czf /files/backups/resitesvn-incr-$date.tar.gz /files/backups/svn


/bin/rm -rf /files/backups/svn
/bin/rm /files/backups/svnchangelog

