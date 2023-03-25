#!/bin/sh
#
# Script for splitting large datasets into more folders
# in order to run parallel on different machines.
#
# Script assumes that you have downloaded the dataset
# and that you have put all root-files into one folder
# (you can use 'MakeNiceStructure.sh' to do this)
#
#
# How To:
# - 0. Put all files into one folder
#      (use MakeNiceStructure.sh)
#
# - 1. Use this script to split the dataset
#      Works only if current folder contains dataset
#      (use scriptname -h for help)
#
# - 2. Distribute splitted dataset to as/ms-machines
#      (use DistributeDataset.sh -h for help)
#
#
# !!! Warning !!!
# The naming scheme of the SgTopD3PDs is assumed
# (i.e. there exit two channels 'el' and 'mu')
# Does not work for an arbitrary data set.
#
# Update 13 TeV: no el/mu anymore, no mv just hardlink for now,
#  so files have to be deleted manually after distribution
#
# ==================================================

if [ "x$1" = "x" ] || [ $1 = "-h" ]; then
    echo
    SCRIPT=`basename $0`
    echo "USAGE: $SCRIPT <dataset you want to split>"
    echo
    exit 1
fi

DATASET=`basename $1`
# test dataset name
if [[ ! -d $DATASET ]]; then
    echo "Dataset does not exist or"
    echo "your current directory does not hold the dataset."
    echo "Please change to the folder that holds the dataset."
    echo "Abort."
    echo
    exit 2
fi

# count number of nominal files
# 1. Due to the naming scheme on the grid one has to
#    keep two different lists for el and mu
# 2. The assumption is that all files of channel (el or mu)
#    have the same number in the file name.
NFILES="$(ls -rv $1/*.root* | wc -l)"

if [ $NFILES -eq 0 ]; then
    echo "No files have been found."
    echo "Dataset already splitted?"
    echo "N = "$NFILES
    echo "Abort."
    exit 5
fi

# Do not split into too many subsets!
# Maximum parallel machines available
#  ms (as): 4 (8)
# --> Total number of cores (8 cores per machine)
#  ms (as): 32 (64)
# --> Split into 32 for now

# new rationale: just spread on the current system, i.e. spread as (ms) samples on as (ms) only
# this means use 32 to leave some room for other samples/other systematics etc (also datm1/dats1 are login machines with only 6 cores)

# save identifier number (one for each channel)
# (if statement for .root.1 files from grid)    
SUFFIX="$(ls -v $1/*.root* | awk -F. '{ if ( $(NF-1) == "root" ) { print $(NF-3) } else { print $(NF-2) }}')"

# Keep the documentation simple!
# (the suffix _EL, _MU is dropped)
#
# ------------------------------------------
# NFILES per core (pay attention to modulo)
# NFILES / NCORES = XFILES + modulo
#
# keep YFILES in 'modulo' directories
# keep XFILES in remaining directories
# ------------------------------------------
NCORES=32
XFILES=`expr $NFILES / $NCORES`
YFILES=`expr $XFILES + 1`
modulo=`expr $NFILES % $NCORES` 

ext=1    # sub dataset number
COPIED=0 # counter for copied files
MADE=0   # counter for created directories

# First do electron loop, then muon loop
# (if you know a clever way to combine, please tell me!)
# new: just one loop anyway
for FILE in $SUFFIX; do
    
    # copy some files
    # ---------------
    if [ $modulo -gt 0 ]; then
	if [ $COPIED -eq $YFILES ]; then
	    ext=`expr $ext + 1`
	    modulo=`expr $modulo - 1`
	    COPIED=1
	else
	    COPIED=`expr $COPIED + 1`
	fi
    else
	if [ $COPIED -eq $XFILES ]; then
	    ext=`expr $ext + 1`
	    COPIED=1
	else
	    COPIED=`expr $COPIED + 1`
	fi
    fi

    # build new directory
    if [ $ext -gt 99 ]; then
	NEWDIR="$DATASET.$ext"
    elif [ $ext -gt 9 ]; then
	NEWDIR="$DATASET.0$ext"
    else
	NEWDIR="$DATASET.00$ext"
    fi

    if [ ! -d $NEWDIR ]; then
	mkdir_cmd="mkdir $NEWDIR"
	echo $mkdir_cmd
	$mkdir_cmd
	MADE=$((MADE+1))
    fi

    for f in $(ls -v $DATASET/*$FILE*.root*); do
	# mv_cmd="mv $f $NEWDIR"
	mv_cmd="cp -l $f $NEWDIR"
	# echo $mv_cmd
	$mv_cmd
    done
done

# some status report
echo "Created $MADE directories"
