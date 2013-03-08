#!/usr/bin/bash
PATH=/usr/mysql/bin:/usr/sfw/sbin:/usr/sfw/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
DIR=/archive/primary/resite/mysql/
DATESTAMP=$(date +%Y%m%d) 
DB_USER=backup            
DB_PASS='r34d0n1y'        
DB_HOST='resite-db2.int.resiteit.com'

# remove backups older than $DAYS_KEEP
DAYS_KEEP=14
find ${DIR} -type f -mtime +$DAYS_KEEP | xargs rm -f

# create backups securely
umask 002

# list MySQL databases and dump each
DB_LIST=`mysql -u $DB_USER -p"$DB_PASS" -h $DB_HOST -e'show databases;'`
DB_LIST=${DB_LIST##Database}
for DB in $DB_LIST;
do
  if [ "${DB}" = "oroValleyWordPress"  -o :${DB} = "resite"  -o :${DB} = "resite"  -o :${DB} = "resiteWordPress"  -o :${DB} = "resiteWordPressNew"  -o :${DB} = "resiteonlineblog"  -o :${DB} = "roundcube"  -o :${DB} = "roundcubemail"  -o :${DB} = "sugarcrm52waspro" ];
  then
    FILENAME=${DIR}${DB}-${DATESTAMP}.sql
    mysqldump -u $DB_USER -p"$DB_PASS" -h $DB_HOST --opt --flush-logs $DB > $FILENAME
    gzip -9 $FILENAME
    # Maybe this will keep it from tripping the slow replication alarm
  else
    continue
  fi
done

