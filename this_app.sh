# set this directory as the current ../APlusPlus
# source this file

# this file
if [[ $SHELL == *"/zsh" ]] ; then
    this_file="${(%):-%x}"
else
    this_file="${BASH_SOURCE[0]}"
fi
echo "Called\n    $this_file"

# follow all symlinks
this_file=$(readlink -f $this_file)

# target directory
this_dir=$(dirname $this_file)
echo "Physical path of this directory is\n    $this_dir"

# check what current ../APlusPlus is
link=$(cd $this_dir/.. && pwd )/APlusPlus
current_dir=$(readlink -f $link)
#echo "Currently link is\n    $link\n  pointing to\n    $current_dir"
if [[ -L "${link}" && -d "${link}" ]]; then
    echo "Current link\n    ${link}"
    if [ "$current_dir" -ef "$this_dir" ]; then
	echo "  already points to this A++ version"
    else
	echo "  points to\n    $current_dir"
	echo "Changing APlusPlus link."
	rm $link
	ln -s $this_dir $link
    fi
else
    echo "${link} is not a symlink to a directory. Need to move manually"
fi
    
    
