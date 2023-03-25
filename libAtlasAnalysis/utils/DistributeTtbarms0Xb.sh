#Script distributes the splittet ttbar folders to ms machines
#!!!ATTENTION!!!
#it is import to execute the MakeMsFolders.sh and the SplitTtbar.sh script 
#!!!ATTENTION!!! please check the number of splittet ttbar folders. The number 
#is hardcoded and stored in the variable i
#USAGE: DistributeTtbarms0Xb.sh TTbarFolderBeforeSplitting

fix=23
i=91
j=`expr $i - $fix `
k=`expr $j - $fix`
l=`expr $k - $fix`

while test $i != $j
  do
  dir="$1.$i"
  echo "cp -r $dir /rdsk/datm8/atlas/SgTopD3PD/mc11c_v10/MC/ttbar"
  cp -r $dir /rdsk/datm8/atlas/SgTopD3PD/mc11c_v10/MC/ttbar
  i=`expr $i - 1`  
done


while test $j != $k
  do
  dir="$1.$j"
  echo "cp -r $dir /rdsk/datm7/atlas/SgTopD3PD/mc11c_v10/MC/ttbar"
  cp -r $dir /rdsk/datm7/atlas/SgTopD3PD/mc11c_v10/MC/ttbar
  j=`expr $j - 1`  
done

while test $k != $l
  do
  dir="$1.$k"
  echo "cp -r $dir /rdsk/datm6/atlas/SgTopD3PD/mc11c_v10/MC/ttbar"
  cp -r $dir /rdsk/datm6/atlas/SgTopD3PD/mc11c_v10/MC/ttbar
  k=`expr $k - 1`  
done

exit 0
