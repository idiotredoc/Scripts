#!/bin/bash
set -e
ARCH=`uname -m`
RELEASE=`cat /etc/redhat-release | awk '{ print substr($3,1,1)}'`

echo "Installing Etc-keeper and prereqs"
echo "Checking for EPEL"
if yum repolist | grep epel; then
echo "EPEL found..."
else
echo "Installing EPEL"
TBI=`curl http://dl/fedoraproject.org/pub/epel/$RELEASE/$ARCH/ | grep -o '>epel-release.*<' | sed -e 's/>//' -e 's/<//'`
rpm -Uvh http://dl.fedoraproject.org/pub/epel/$RELEASE/$ARCH/$TBI
fi
echo "Installing etckeeper, bzr"
yum install etckeeper bzr -y
echo "Initializing git repo"
cd /etc
etckeeper init
echo "First commit!"
etckeeper vcs commit -m "First commit"
echo "Now check the status..." 
etckeeper vcs status
