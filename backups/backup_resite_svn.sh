#!/bin/bash
DATE=`/bin/date '+%Y%m%d'`
DIR=/archive/primary/resite/svn
WHICHTYPE="$1"
if [ -z "${WHICHTYPE}" ];
then
        WHICHTYPE="weekly"
fi
if [ "${WHICHTYPE}" -eq "weekly" ];
then
        /usr/local/bin/svn export -q http://svn.dev.residentsource.com/svn/rit/trunk/rit $DIR/rit
        /usr/local/bin/svn export -q http://svn.dev.residentsource.com/svn/rit/trunk/clientdev $DIR/clientdev 
        /usr/local/bin/svn export -q http://svn.dev.residentsource.com/svn/rit/projects/core/trunk  $DIR/core-trunk
fi
(find $DIR -type f -ctime +14 | xargs rm ) > /dev/null 2>&1

