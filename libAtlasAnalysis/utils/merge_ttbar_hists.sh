#!/bin/sh
if [ "x$1" = "x" ]; then
    echo "USAGE: merge_ttbar_hists.sh [arg]"
    echo "Merges all ttbar files ttbar*[arg]_hists.root ! Exception: nominal (no suffix used)."
    echo "Provide systematics mode as argument [arg]!"
else
# nominal: no suffix
    if [ "$(echo $1)" = "nominal" ]; then
	infiles=`ls -1 ttbar*.root | grep -v evgen | grep -v psgen | grep -v ifsr | paste -s `
	if [ "x$infiles" = "x" ]; then
	    echo "Provide files ttbar*.root !"
	else
	    echo $infiles
	    hadd -f ttbar_incl_hists.root $infiles
	fi
# systematics
    else
	infiles=`ls -1 ttbar*$1_hists.root | paste -s `
	if [ "x$infiles" = "x" ]; then
	    echo "Provide files ttbar*$1*.root !"
	else
	    echo $infiles
	    hadd -f9 ttbar_incl_$1_hists.root $infiles               
	fi
    fi
fi
