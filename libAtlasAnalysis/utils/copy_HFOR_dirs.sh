#!/bin/sh
for i in $(cat $LIBSINGLETOP/tasks/SampleListMC11c_as0Xb_syst.txt | grep wjets | grep -v \# | awk '{print $3}');
do
    inputmark=$i/HFOR;outputmark=`echo $inputmark | awk -F/ '{print $10}'`;
    outputdir=`cat $LIBSINGLETOP/tasks/SampleListMC11c_ms0Xb_syst.txt | grep $outputmark | awk '{print $3}'`;
    cp -r $i/HFOR $outputdir;
    
    echo "copied" $i/HFOR to $outputdir;
    
done;