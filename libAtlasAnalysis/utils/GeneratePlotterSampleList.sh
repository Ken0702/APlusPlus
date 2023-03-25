#!/bin/sh
#
# Script which produces from the InputDataset the InputDataset list for 
# the plotter. 
# For this script the same dataset list can be used which is also used for 
# producing the analysis tasks. 
# Mind that the order of the InputDatasetList will be the same for the 
# stacked plots.
#

if [ "x$1" = "x" ] || [ $1 = "-h" ]; then
    echo
    SCRIPT=`basename $0`
    echo "USAGE: $SCRIPT <output filename> <list of input datasets> <data lumi> <path to x-section file>"
    echo
    exit 1
fi

if [ "x$1" = "x" ]; then
    echo
    echo "No output filename given"
    echo
    exit 1
fi
SampleList=$1

if [ "x$2" = "x" ]; then
    echo
    echo "No dataset list provided"
    echo
    exit
fi
DatasetList=$2

if [ "x$3" = "x" ]; then
    echo
    echo "No data lumi provided"
    echo "20339.8 will be used"
    echo
    DataLumi="20339.8"
fi
DataLumi=$3

if [ "x$4" = "x" ]; then
    echo
    echo "No x-section file provided"
    echo "~/atlas/RootCore/TopDataPreparation/data/XSection-MC12-8TeV.data will be used"
    echo
    XSectionFile=$(echo ~/atlas/RootCore/TopDataPreparation/data/XSection-MC12-8TeV.data)
fi
XSectionFile=$4

if [ -e $SampleList ]; then
    rm $SampleList
    echo "Old Sample List deleted"
fi 
touch $SampleList
echo "Create new Sample List: '"$SampleList"'"



OldProcess=""
OldProcLong=""
Color=""
# read process and dataset number
for Line in $(cat $DatasetList | grep -v "^#"); do
    Process=$(echo $Line | awk -F,  '{print $1}')
    ProcLong=$(echo $Line | awk -F,  '{print $2}')
    SSID=$(echo $Line | awk -F,  '{print $3}')

    printf .


    if [ "$Process" != "$OldProcess" ]; then
	echo "# " >> $SampleList
	echo "# "$Process >> $SampleList
    fi

    if [ "$Process" = "data" ]; then
	if [ "$ProcLong" = "Egamma" ]; then
	    echo "data Egamma_incl "$DataLumi" kBlack" >> $SampleList
	fi
	if [ "$ProcLong" = "Muons" ]; then
	    echo "data Muons_incl "$DataLumi" kBlack" >> $SampleList
	fi

    else 
	XSection=$( echo $(cat $XSectionFile | grep $SSID | awk -F" "  '{print $2*$3}' ) )

	if [ "$ProcLong" != "$OldProcLong" ]; then
	    
	    if [ "$Process" = "sgtop" ]; then
		Color="kAzure+5"
	    fi
	    if [ "$Process" = "diboson" ]; then
		Color="kOrange+4"
	    fi
	    if [ "$Process" = "zjets" ]; then
		Color="kOrange+1"
	    fi
	    if [ "$Process" = "wjetsLF" ]; then
		Color="kGreen-7"
	    fi
	    if [ "$Process" = "wjetsHF" ]; then
		Color="kGreen-3"
	    fi
	    if [ "$Process" = "ttbar" ]; then
		Color="kRed-4"
		ProcLong="ttbar_incl"
	    fi
		echo $Process" "$ProcLong" "$XSection" "$Color >> $SampleList

	fi
    fi


    OldProcess=$Process
    OldProcLong=$ProcLong

done

