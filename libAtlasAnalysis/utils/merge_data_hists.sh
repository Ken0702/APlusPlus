#!/bin/sh
if [ "x$1" = "x" ]; then
    echo "USAGE: merge_data_hists.sh [arg]"
    echo "Merges all data files [arg]*_hists.root ! "
    echo "Provide data mode as argument [arg]! Choices: \"Egamma\" or  \"Muon\""
else
# nominal: no suffix
    infiles=`ls -1 $1_*.root |  paste -s `
    if [ "x$infiles" = "x" ]; then
	echo "Provide files $1*.root !"
    else
	echo $infiles
	hadd -f9 $1_incl_hists.root $infiles
    fi
fi
