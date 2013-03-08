#!/bin/sh

/bin/mkdir /files/backups/mysql
/bin/chmod 777 /files/backups/mysql
/bin/su backup -c "/bin/cp /backups/resite/mysql/*`date '+%y%m%d'`* /files/backups/mysql"
/bin/chown root /files/backups/mysql/*.sql.gz
/bin/tar cvf /files/backups/resiteDB-`date '+%Y%m%d'`.tar.gz /files/backups/mysql

/bin/rm -rf /files/backups/mysql


