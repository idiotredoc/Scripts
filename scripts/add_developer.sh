#!/bin/bash
# Add new developer account for RSO environment
# Usage: $0 <username> <password>
USER=$1
PASS=$2
qry1="create user '$USER'@'%' identified by '$PASS';"
qry2="grant all on resite_daily.* to '$USER'@'%;"
qry3="flush privileges;"
echo "Adding user $USER"
useradd -G rscdev -p $PASS $USER
echo "Chowning the home dir"
chown -R $USER:rscdev /home/$USER
echo "Adding to Samba"
echo $PASS; echo $PASS | smbpasswd -s $USER
echo "Adding to SVN"
htpasswd -b /usr/local/apache2/conf/svn.passwd $USER $PASS
echo "Editing smb.conf..."
echo "[$USER]
   comment = $USER
   path = /home/$USER
   public = no
   writeable = yes
   printable = no
   valid users = $USER
   create mask = 0775" >> /etc/samba/smb.conf
echo "Reloading Samba"
/etc/init.d/smb restart
echo "Logging into mysql"
echo "Executing queries: "
echo $qry1
echo $qry2
echo $qry3

/usr/bin/mysql -u root << eof
$qry1
$qry2
$qry3
eof

echo "All done! User $USER is all set up." 
