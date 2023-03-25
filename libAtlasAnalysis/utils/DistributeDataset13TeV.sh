#!/bin/sh
#
# usage: use option -h for help
#        can only be used for datasets that have
#        been splitted with SplitTtbar.sh
#
# ==================================================

# WARNING: current rsync version creates additional unwanted subdirectories
# edit: appended / on source, SHOULD work now; otherwise use something like
#   for dir in user.cescobar.*410470.*; do mv $dir/$dir/* $dir; done
#   find . -type d -empty -delete
# to move files and clean empty directories

#
# Check Inputs
# ------------
if [ "x$1" = "x" ] || [ $1 = "-h" ]; then
    echo
    SCRIPT=`basename $0`
    echo "USAGE: $SCRIPT <dataset you want to distribute (w/o .xxx)> <machines: as or ms> <machine numbers colon separated (e.g 1:4:5)>"
    echo
    echo "<machine numbers>: Current machine should be included in list. Otherwise there will be no datasets left on this machine."
    echo "WARNING: Files will be copied from as->ms / ms->as but moved from as->as / ms->ms. --> Copy first, then move!"
    echo
    exit 1
fi

# test dataset name
DATASET=`basename $1`
if [[ ! -d $DATASET ]]; then
    echo "Dataset does not exist or"
    echo "your current directory does not hold the dataset."
    echo "Please change to the folder that holds the dataset."
    echo "Abort."
    echo
    exit 2
fi

if [[ ! -d $DATASET".001" ]]; then
    echo "Please provide a dataset name without .xx suffix"
    echo "Abort."
    echo
    exit 3
fi

if [ "x$2" = "x" ]; then
    echo "No machine specified."
    echo "Abort."
    echo
    exit 6
fi
dest_machine=$2

if [ "x$3" = "x" ]; then
    echo "No machines specified."
    echo "Abort."
    echo
    exit 6
fi

#
# check environment
# -----------------
source_path=$PWD

# check rdsk
ISRDSK=$(echo $source_path | awk -F"/" '{ print $2 }')
source_disk=$(echo $source_path | awk -F "/" '{ print $3 }')

if [ $ISRDSK != "rdsk" ]; then
    echo "Script only works inside an /rdsk folder"
    echo "Abort."
    echo
    exit 4
fi

# get hostname (machine)
MSAS=`hostname`
case $MSAS in
    ms*)
    MSAS="ms"
    ;;
    as*)
    MSAS="as"
    ;;
    *)
    echo "Not on as or ms machine."
    echo "Abort."
    echo
    exit 10
    ;;
esac

#
# mirror folders on as machines
# -----------------------------

# count datasets
NFILES=$(ls -d $DATASET.*/ | wc -l)
NDISKS=$(echo $3 | awk -F":" '{print NF}')
echo "'$NFILES' datasets to be distributed on '$NDISKS' disks"

# calculate no. of sub-datasets for each disk
# NFILES / NDISKS = XFILES + modulo
# copy XFILES + 1 until modulo is reached
XFILES=`expr $NFILES / $NDISKS`
modulo=`expr $NFILES % $NDISKS` 

COPIED=0 # counter for copied files

for disk in $(echo $3 | awk -F":" '{ for(i=1;i<=$NF;i++) print $i }'); do

    # create folder
    # -------------
    if [ $dest_machine = "as" ]; then
	target_disk="dats$disk"
	echo "Source directory: $source_path"
	echo "Source disk.....: $source_disk"
	echo "Target disk.....: $target_disk"
	folder=$(echo $source_path | sed "s/$source_disk/$target_disk/")
    else
	target_disk="datm$disk"
	echo "Source directory: $source_path"
	echo "Source disk.....: $source_disk"
	echo "Target disk.....: $target_disk"
	folder=$(echo $source_path | sed "s/$source_disk/$target_disk/")
    fi
    mkdir_cmd="mkdir -p $folder"
    echo $mkdir_cmd
    $mkdir_cmd
    
    # copy some files
    # ---------------
    if [ $modulo -gt 0 ]; then
	EXTRA=1
	modulo=`expr $modulo - 1`
    else
	EXTRA=0
    fi
    
    # calculate number of datasets to be copied
    TOCOPY=`expr $XFILES + $EXTRA`
    echo "copy '$TOCOPY' directories"

    RSFLAGS=" -avh --progress "
    RSRM=" --remove-source-files "
    step=1
    while [ $step -le $TOCOPY ]; do
	COPIED=`expr $COPIED + 1`
	if [ $COPIED -le 9 ]; then
	    source_folder="$DATASET.00$COPIED"
	    target_folder="$folder/$DATASET.00$COPIED"
	elif [ $COPIED -le 99 ]; then
	    source_folder="$DATASET.0$COPIED"
	    target_folder="$folder/$DATASET.0$COPIED"
	else
	    source_folder="$DATASET.$COPIED"
	    target_folder="$folder/$DATASET.$COPIED"
	fi

	if [ "$source_disk" = "$target_disk" ]; then
	    if [ $COPIED -le 9 ]; then
		echo "Keep $DATASET.00$COPIED on $source_disk"
	    elif [ $COPIED -le 99 ]; then
		echo "Keep $DATASET.0$COPIED on $source_disk"
	    else
		echo "Keep $DATASET.$COPIED on $source_disk"
	    fi
	else 
	    # if [ -d $target_folder ]; then
	    # 	echo "Done. Skip: $target_folder"
	    # else
		if [ $MSAS = $dest_machine ]; then
		    mv_cmd="rsync $RSFLAGS $RSRM $source_folder/ $target_folder"
		    #mv_cmd="mv $source_folder $target_folder"
		    echo $mv_cmd
		    $mv_cmd
		    chmod g+w -R $target_folder
		else
		    cp_cmd="rsync $RSFLAGS $source_folder/ $target_folder"
		    #cp_cmd="cp -r $source_folder $target_folder"
		    echo $cp_cmd
		    $cp_cmd
		    chmod g+w -R $target_folder
		fi
#	    fi
	fi
	step=`expr $step + 1`
    done
done
echo "Total number of directories copied......: '$COPIED'"
echo "Total number of directories to be copied: '$NFILES'"
echo "Removing empty source directories"
find $source_path -depth -type d -empty -delete

exit 0
