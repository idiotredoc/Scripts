#!/usr/bin/bash
PATH=/usr/mysql/bin:/usr/sfw/sbin:/usr/sfw/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
DIR=/archive/primary/resite/mysql/
DATESTAMP=$(date +%Y%m%d) 
DB_USER='backup'           
DB_PASS='r34d0n1y'        
DB_HOST='resite-db2.int.resiteit.com'
INITTIME=$(date +%S)
ERRMSG="Backup failed - file does not exist!"
day=`/bin/date '+%d'`
if [ $day -eq 1 ];
then
        echo "It's the first of the month! Dont run."
        exit
fi

# remove backups older than $DAYS_KEEP
DAYS_KEEP=14
find ${DIR} -type f -mtime +$DAYS_KEEP | xargs rm -f

# create backups securely
umask 002
DB_LIST=(resite resiteWordPress resiteWordPressNew resiteonlineblog roundcube roundcubemail sugarcrm52waspro)
echo "Backing up DB schemas: "
for DB in ${DB_LIST[*]};
do
  echo "$DB"
  FILENAME=${DIR}${DB}-${DATESTAMP}.sql
  mysqldump -u $DB_USER -p"$DB_PASS" -h $DB_HOST --opt --flush-logs $DB > $FILENAME
  gzip -9 $FILENAME
  if [ ! -f $FILENAME.gz ]; then
    echo $ERRMSG
  fi
done
ENDTIME=$(date +%S)
TTIME=$(( $ENDTIME - $INITTIME ))
echo "Total runtime: $TTIME seconds"
