#!/bin/bash
# Script to handle starting / stopping AWS instances

if [ $# -lt 2 ]
then 
	echo "Usage: $0 start/stop one/all/flagged"
	exit
fi

# start / stop
COMMAND=$1 
# all / flagged
OPT=$2
# file contianing instance ids and start/stop flags
INSTANCEFILE=/home/nlutz/ec2_instances

case "$OPT" in

one) if [ $COMMAND == start ]
     then
        ec2-describe-instances | grep -v Function | awk '/Name/ {print $3" "$5}'
        echo "Enter the ID of the instance you'd like to start"
        # get list of instances 
	read INSTANCEID
        echo "Starting $INSTANCEID!"
	ec2-start-instances $INSTANCEID 
	elif [ $COMMAND == stop ]
     then
        ec2-describe-instances | grep -v Function | awk '/Name/ {print $3" "$5}'
	echo "Enter the ID of the instance you'd like to stop"
        # get list of instances
        read INSTANCEID
        echo "Stopping $INSTANCEID!"
        ec2-stop-instances $INSTANCEID
     else
        echo "I don't recognize $COMMAND. Exiting!"
        exit
      fi
      ;;
all) if [ $COMMAND == start ]
     then
	echo "starting all AWS instances!"
	# get list of instances and boot them 
	while read -r INSTANCEID FLAG; do
		echo "starting $INSTANCEID "
	        ec2-start-instances $INSTANCEID
	done < $INSTANCEFILE
     elif [ $COMMAND == stop ] 
     then
	echo "stopping all AWS instances!"
        # get list of instances and stop them
        while read -r INSTANCEID FLAG; do
                echo "stopping $INSTANCEID "
	        ec2-stop-instances $INSTANCEID
        done < $INSTANCEFILE
     else
	echo "I don't recognize $COMMAND. Exiting!"
	exit
      fi
      ;;
flagged) if [ $COMMAND == start ]
         then
	    echo "starting flagged AWS instances!"
            # get list of instances and boot them
            while read -r INSTANCEID FLAG; do
     		   if [ $FLAG == AUTO ] 
		   then
		   	   echo "starting $INSTANCEID "
		           ec2-start-instances $INSTANCEID
 		   fi
            done < $INSTANCEFILE
         elif [ $COMMAND == stop ] 
	 then
            echo "stopping flagged AWS instances!"
            # get list of instances and stop them
            while read -r INSTANCEID FLAG; do
	   	   if [ $FLAG == AUTO ] 
		   then
                      echo "stopping $INSTANCEID "
     	              ec2-stop-instances $INSTANCEID
                   fi
            done < $INSTANCEFILE
         else
            echo "I don't recognize $COMMAND. Exiting!"
            exit
         fi
	 ;;
*) echo "$OPT not recognized. Exiting!"
   exit
   ;;
esac
