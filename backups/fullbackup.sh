#!/bin/sh

date=`/bin/date '+%Y%m%d'`

/bin/tar -cpzf /files/backups/clientSites-$date.tar.gz /var/www --exclude "/var/www/desi" --exclude "/var/www/images" --exclude "/var/www/cache" --exclude "/var/www/toBeDeleted" --exclude "/var/www/coreBeta" --exclude "/var/www/coreStaging" --exclude "/var/www/coreTest"

/bin/tar -cpzf /files/backups/apacheConf-$date.tar.gz /etc/httpd


