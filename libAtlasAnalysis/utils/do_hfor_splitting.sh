#!/bin/sh
if [ "x$1" = "x" ] || [ "x$2" = "x" ]; then
    echo "USAGE: do_hfor_splitting.sh [arg1] [arg2]"
    echo "Split all *.root files in the directories listed in the given sample list according to hfor type of the events and collect them in given output folder!"
    echo "Provide sample list as argument [arg1] and name of output folder as argument [arg2]!"  

submitlog=submit.log           # Name of the logfile
submitlog=`pwd`/$submitlog
else
    for a in $(cat $1 | grep wjets | grep -v \# | awk '{print $3}'); do
	echo $a;
	cd $a;
	ls -l;
	mkdir $2;
	$LIBATLASANALYSIS/utils/hfor_splitall.sh $a $2 | tee -a $submitlog;
    done
fi

# Exit

echo "All done !" 2>&1 | tee -a $submitlog