#!/bin/sh

if [ "$1" = "-h" ]; then
   echo
   Script=`basename $0`
   echo "USAGE: $Script <downloadlist>"
   echo
   exit 0
fi

if [ "x$1" = "x" ]; then
    echo
    echo "No download list provided!"
    echo
    exit
fi

DatasetList=$1

for ds in $(cat $DatasetList | grep -v "#"); do
    ds2=$(echo $ds | awk -F: '{ print $2}' )
    usercheck=$(echo $ds2 | awk -F. '{ print $1}' )

    if [ $usercheck != "user" ]; then
	echo "Dataset name does not begin with 'user'!"
	echo "Abort!"
	exit 1
    fi

    # loop over replicas
    Replicas=$(ls -d $(echo $(basename $ds2)"*/"))
    for rep in $Replicas; do
	
	# loop over files in replicas
	for file in $(find $(echo $(basename $rep)"/") -type f); do
	    newfile=$(echo $(basename $file) | awk -F. '{ print $5".root" }')
	    # move files
	    echo mv $file $newfile
	    mv $file $newfile
	    echo
	done
	# remove replica folder
	echo "rm -r $rep"
	rm -r $rep
    done
done

exit 0
