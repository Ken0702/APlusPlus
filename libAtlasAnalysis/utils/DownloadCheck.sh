#!/bin/sh
#
#
# Script for checking locally downloaded files against grid datasets
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
Incomplete="IncompleteDatasets.txt"

for ds in $(cat $DatasetList); do
    #
    # First: Check if all files are downloaded
    #
    
    touch $Incomplete

    # Get Number of files in dataset and locally
    NGridFiles=$(dq2-ls -n $ds | awk -F" " '{ print $1 }')
    NLocalFiles=$(find $(echo $(basename $ds)"*/") -type f | wc -l)

    if [ $NGridFiles -eq $NLocalFiles ]; then
	echo "Dataset complete: $ds"
    else
	echo "Dataset incomplete: $ds"
	echo $ds >> $Incomplete
    fi
done

NIncomplete=$(cat $Incomplete | wc -l)
echo "------------------------------------------"
echo "Summary: Number of incomplete datasets: $NIncomplete"
echo 

if [ $NIncomplete -eq 0 ]; then
    # Remove file and leave
    rm $Incomplete
    exit 0
fi

echo "Missing files:"

LocalFiles="LocalFiles.txt"
GridFiles="GridFiles.txt"

for ds in $(cat $Incomplete); do

    touch $LocalFiles
    touch $GridFiles

    DQ2Output=$(dq2-ls -f $ds)
    # get files from DQ2 output
    echo -e "$DQ2Output" | cut -f 2 | sort | grep -v "total files" | grep -v "total size" | grep -v "date" | grep -v "local files" >> $GridFiles

    # get local files
    find $(echo $(echo $(basename $ds)"*/")) -type f -printf "%f\n" >> $LocalFiles
    
    echo 
    echo "Dataset: "$ds
    cat $LocalFiles $GridFiles | sort | uniq -u | grep -v $ds
    
    rm $LocalFiles
    rm $GridFiles
done

rm $Incomplete