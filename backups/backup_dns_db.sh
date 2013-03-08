#!/bin/bash
DIR=/root/mysql-backups/
DATESTAMP=$(date +%Y%m%d)
MAIL=systems@resiteonline.com
ERRMSG="There has been a problem backing up $FILENAME on $DATESTAMP from dsrscns01"
# remove backups older than $DAYS_KEEP
DAYS_KEEP=14
find ${DIR} -type f -mtime +$DAYS_KEEP | xargs rm -f

# create backups securely
umask 002

# Backup powerdns external and internal db
DBLIST=(powerdns pdns_ds_int)
for DB in ${DBLIST[*]}; do
  FILENAME=${DIR}${DB}-${DATESTAMP}.sql
  mysqldump  --opt --flush-logs $DB > $FILENAME
  gzip -9 $FILENAME
  if [ ! -f $FILENAME.gz ]; then
    echo $ERRMSG
  fi
done

