#!/bin/bash
echo "Installing Etc-keeper and prereqs"
sudo yum install etckeeper bzr -y
echo "Initializing git repo"
cd /etc
sudo etckeeper init
echo "First commit!"
sudo etckeeper vcs commit -m "First commit"
echo "Now check the status..." 
sudo etckeeper vcs status
