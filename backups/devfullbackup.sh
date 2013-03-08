#!/bin/sh

date=`/bin/date '+%Y%m%d'`

#/usr/bin/mysqldump -u root -pzk4hc3t -h 10.93.72.129 resite | gzip > /files/backups/resiteDB-`date '+%m-%d-%Y'`.sql.gz

/bin/tar -pczf /files/backups/archive-$date.tar.gz /archive/leasing-applications /archive/recolor /archive/uploads

/bin/rm -rf /files/backups/svn

/usr/local/bin/svn export -q http://svn.dev.residentsource.com/svn/rit/trunk/rit /files/backups/svn/rit
/usr/local/bin/svn export -q http://svn.dev.residentsource.com/svn/rit/trunk/clientdev /files/backups/svn/clientdev
/usr/local/bin/svn export -q http://svn.dev.residentsource.com/svn/rit/projects/core/trunk /files/backups/svn/core-trunk



/bin/tar czf /files/backups/resitesvn-$date.tar.gz /files/backups/svn

/bin/rm -rf /files/backups/svn

#/bin/mkdir /files/backups/mysql
#/bin/chmod 777 /files/backups/mysql
#/bin/su backup -c "/bin/cp /backups/resite/mysql/*`date '+%y%m%d'`* /files/backups/mysql"
#/bin/chown root /files/backups/mysql/*.sql.gz

