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
NFILES_EL="$(ls -rv $1/*el.root* | wc -l)"
NFILES_MU="$(ls -rv $1/*mu.root* | wc -l)"

if [ $NFILES_EL -eq 0 ] || [ $NFILES_MU -eq 0 ]; then
    echo "For at least one channel, no files have been found."
    echo "Dataset already splitted?"
    echo "N_el = "$NFILES_EL
    echo "N_mu = "$NFILES_MU
    echo "Abort."
    exit 5
fi

if [[ $NFILES_EL -ne $NFILES_MU ]]; then
    echo "The number of el and mu files should be the same!"
    echo "Please check the number of files!"
    echo "The script assumes that the numbers are equal"
    echo "for each channel! I don't know what to do!"
    echo "Abort!"
    exit 6
fi


# Do not split into too many subsets!
# Maximum parallel machines available
#  ms (as): 4 (8)
# --> Total number of cores (8 cores per machine)
#  ms (as): 32 (64)
# --> Split into 32 for now

# save identifier number (one for each channel)
# (if statement for .root.1 files from grid)    
SUFFIX_EL="$(ls -v $1/*el.root* | awk -F. '{ if ( $(NF-2) == "el" ) { print $(NF-3) } else { print $(NF-2) }}')"
SUFFIX_MU="$(ls -v $1/*mu.root* | awk -F. '{ if ( $(NF-2) == "mu" ) { print $(NF-3) } else { print $(NF-2) }}')"

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
XFILES_EL=`expr $NFILES_EL / $NCORES`
YFILES_EL=`expr $XFILES_EL + 1`
modulo_EL=`expr $NFILES_EL % $NCORES` 

XFILES_MU=`expr $NFILES_MU / $NCORES`
YFILES_MU=`expr $XFILES_MU + 1`
modulo_MU=`expr $NFILES_MU % $NCORES` 

ext=1    # sub dataset number
COPIED=0 # counter for copied files

# First do electron loop, then muon loop
# (if you know a clever way to combine, please tell me!)
for FILE in $SUFFIX_EL; do
    
    # copy some files
    # ---------------
    if [ $modulo_EL -gt 0 ]; then
	if [ $COPIED -eq $YFILES_EL ]; then
	    ext=`expr $ext + 1`
	    modulo_EL=`expr $modulo_EL - 1`
	    COPIED=1
	else
	    COPIED=`expr $COPIED + 1`
	fi
    else
	if [ $COPIED -eq $XFILES_EL ]; then
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
	mkdir $NEWDIR
	echo "mkdir $NEWDIR"
    fi

    for f in $(ls -v $DATASET/*$FILE*el*.root*); do
	mv $f $NEWDIR
    done
done

ext=1    # sub dataset number
COPIED=0 # counter for copied files
for FILE in $SUFFIX_MU; do
    
    # copy some files
    # ---------------
    if [ $modulo_MU -gt 0 ]; then
	if [ $COPIED -eq $YFILES_MU ]; then
	    ext=`expr $ext + 1`
	    modulo_MU=`expr $modulo_MU - 1`
	    COPIED=1
	else
	    COPIED=`expr $COPIED + 1`
	fi
    else
	if [ $COPIED -eq $XFILES_MU ]; then
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
	mkdir $NEWDIR
	echo $NEWDIR
    fi

    for f in $(ls -v $DATASET/*$FILE*mu*.root*); do
	mv $f $NEWDIR
    done
done
