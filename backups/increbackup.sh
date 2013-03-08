#!/bin/sh

date=`/bin/date '+%Y%m%d'`

/bin/tar -cpzf /files/backups/apacheConf-$date.tar.gz /etc/httpd

