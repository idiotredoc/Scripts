#!/bin/bash
INITTIME=$(date +%S)
/bin/echo "Rsync of /files beginning..."
/usr/bin/rsync -zxa --exclude='.zfs' --exclude='.snapshot' --delete rscdev01:/files/ /archive/primary/resite/files/

export date=`date '+%Y%m%d%H'`
/bin/echo "ZFS snapshot beginning..."
/usr/sbin/zfs snapshot archive/primary@${date}

# Remove oldest backup
echo "Remiving oldest snapshot (28 days)"
export btr=`zfs list | grep archive/primary | head -n 28 | tail -1 | cut -d\  -f1`

/usr/sbin/zfs destroy $btr
ENDTIME=$(date +%S)
TTIME=$(( $ENDTIME - $INITTIME ))
echo "Total runtime: $TTIME seconds"
