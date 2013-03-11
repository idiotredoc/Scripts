#!/bin/bash
ARCH=`uname -m`
RELEASE=`cat /etc/redhat-release | awk '{ print substr($3,1,1)}'`
TBI=epel-release-5-4.noarch.rpm

echo "Installing Etc-keeper and prereqs"
echo "Checking for EPEL"
repolist | grep epel
if [ $? == 1]; then
echo "Installing EPEL"
sudo rpm -Uvh http://dl.fedoraproject.org/pub/epel/$RELEASE/$ARCH/$TBI
fi
echo "Installing etckeeper, bzr"
sudo yum install etckeeper bzr -y
echo "Initializing git repo"
cd /etc
sudo etckeeper init
echo "First commit!"
sudo etckeeper vcs commit -m "First commit"
echo "Now check the status..." 
sudo etckeeper vcs status
