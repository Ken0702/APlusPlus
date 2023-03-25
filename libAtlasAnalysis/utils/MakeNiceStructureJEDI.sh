#!/bin/sh
#
# Merge replicas (dataset).replicaNr/ into one folder
#
# List of datasets have to be provided
#

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
    usercheck=$(echo $ds | awk -F. '{ print $1}' )

    if [ $usercheck != "user" ]; then
	echo "Dataset name does not begin with 'user'!"
	echo "Abort!"
	exit 1
    fi

    tid=$(echo $ds | awk -F'_' '{ print $NF}')

    # can be electron or muon dataset
    case $tid in 
	(*el.root/) echo $ds
	;;
	(*mu.root/) echo $ds
	;;
	(*el_*.root/) echo $ds
	;;
	(*mu_*.root/) echo $ds
	;;
	(*)
	echo "Something is wrong with the dataset name!"
	echo "Abort!"
	echo $tid
	exit 2
	;;
    esac
    
    # create the new dataset
    newdir=$(echo $ds | sed -e "s|_$tid|/|g")

    echo
    echo "Create Folder: "$newdir
    mkdir -p $newdir
    echo 
    
    # loop over replicas
    Replicas=$(ls -d $(echo $(basename $ds)"*/"))

    for rep in $Replicas; do
	echo "mv $rep* $newdir"
	echo
	# move files
	mv $rep* $newdir
	# remove replica folder
	rm -r $rep
    done
done

exit 0


