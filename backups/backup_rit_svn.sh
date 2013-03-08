#!/bin/bash
WHICHTYPE="$1"
if [ -z "${WHICHTYPE}" ];
then
        WHICHTYPE="weekly"
fi
LOCALDIR="/archive/primary/resite/svn"
REMOTEDIR="/archive/primary/remote/resite/svn"
FILENAME=`date +"${LOCALDIR}/svndump-${WHICHTYPE}.gz"`
ssh rscdev02.int.residentsource.com 'sudo svnadmin dump -q /var/lib/svn/rit' | gzi
p -9 > ${FILENAME}
if [ "${WHICHTYPE}" -eq "weekly" ];
then
        cp ${FILENAME} ${REMOTEDIR}/
fi
(find . -type f -ctime +14 | xargs rm ) > /dev/null 2>&1

