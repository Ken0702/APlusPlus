#!/bin/sh
files=`ls -1`
for ds in $(echo $files);do
    usercheck=$(echo $ds | awk -F. '{ print $1}' )
    if [ $usercheck = "user" ]; then
	tid=$(echo $ds | awk -F. '{ print $8}' )
	if [ "x$tid" != "x" ]; then
	    newdir=$(echo $ds | sed -e "s/.$tid//g")
	    test=`find -name $newdir`
	    if [ "x$test" = "x" ]; then
		mkdir $newdir
	    fi
            #test whether folder is empty
	    if [ $(ls -1 $ds | wc -l) != 0 ]; then
		mv $ds/* $newdir
	    else
		echo "This folder was empty: ' "$ds " ' "
	    fi
	    rm -r $ds
	fi
    fi
done

exit 0


