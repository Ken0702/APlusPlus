#!/bin/sh
#
# HFOR splitting for all files in the given directory The output
# files are written to $out_folder (which is a relative path)
#
if [ "x$1" = "x" ] || [ "x$2" = "x" ]; then
    echo "USAGE: hfor_splitall.sh [arg1] [arg2]"
    echo "Split all *.root files in the given directory according to hfor type of the events and collect them in given output folder!"
    echo "Provide input directory as argument [arg1] and name of output folder as argument [arg2]!"  
else
    out_folder=$2
    
    for i in $1/*.el*.root* $1/*.mu*.root*; do
	cat > hforsplit_run.C << _EOF_
{
gROOT->LoadMacro("$LIBATLASANALYSIS/utils/hfor_split.C");
hfor_split("$i", "$out_folder");
}
_EOF_
	root -l -b -q hforsplit_run.C
    done
    rm -f hforsplit_run.C
fi
