#!/usr/bin/bash
PATH=/usr/mysql/bin:/usr/sfw/sbin:/usr/sfw/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
DIR=/archive/primary/resite/mysql/powerdns
DATESTAMP=$(date +%Y%m%d) 
day=`/bin/date '+%d'`
if [ $day -eq 1 ];
then
        echo "It's the first of the month! Dont run."
        exit
fi

INITTIME=$(date +%S)
ERRMSG="There has been a problem backing up $FILENAME on $DATESTAMP from dsrscns01"
DB_HOST='dsrscns01.int.resiteit.com'

# remove backups older than $DAYS_KEEP
DAYS_KEEP=14
find ${DIR} -type f -mtime +$DAYS_KEEP | xargs rm -f

# create backups securely
umask 002
echo "Copying DNS backup from dsrscns01..."
# Backup powerdns external and internal db
DBLIST=(powerdns pdns_ds_int)
for DB in ${DBLIST[*]}; do
  FILENAME=${DB}-${DATESTAMP}.sql.gz
  scp -r dsrscns01:/home/backup/mysql-backups/$FILENAME $DIR 
  if [ ! -f $DIR/$FILENAME ]; then
    echo $ERRMSG
  fi
done
ENDTIME=$(date +%S)
TTIME=$(( $ENDTIME - $INITTIME ))
echo "Total runtime: $TTIME seconds"

