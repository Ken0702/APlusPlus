#Script distributes the splittet ttbar folders to ms machines
#!!!ATTENTION!!!
#it is import to execute the MakeMsFolders.sh and the SplitTtbar.sh script
#!!!ATTENTION!!! please check the number of splittet ttbar folders. The number
#is hardcoded and stored in the variable i
#USAGE: DistributeTtbar.sh TTbarFolderBeforeSplitting

fix=12
fox=11
i=91
j=`expr $i - $fix`
k=`expr $j - $fix`
l=`expr $k - $fix`
m=`expr $l - $fox`
n=`expr $m - $fox`
o=`expr $n - $fox`
p=`expr $o - $fox`
q=`expr $p - $fox`

while test $i != $j
  do
  dir="$1.$i"
  echo "cp -r $dir /rdsk/dats8/atlas/SgTopD3PD/mc11c_v10/MC/ttbar"
  cp -r $dir /rdsk/dats8/atlas/SgTopD3PD/mc11c_v10/MC/ttbar
  i=`expr $i - 1`  
done


while test $j != $k
  do
  dir="$1.$j"
  echo "cp -r $dir /rdsk/dats7/atlas/SgTopD3PD/mc11c_v10/MC/ttbar"
  cp -r $dir /rdsk/dats7/atlas/SgTopD3PD/mc11c_v10/MC/ttbar
  j=`expr $j - 1`  
done

while test $k != $l
  do
  dir="$1.$k"
  echo "cp -r $dir /rdsk/dats6/atlas/SgTopD3PD/mc11c_v10/MC/ttbar"
  cp -r $dir /rdsk/dats6/atlas/SgTopD3PD/mc11c_v10/MC/ttbar
  k=`expr $k - 1`  
done

while test $l != $m
  do
  dir="$1.$l"
  echo "cp -r $dir /rdsk/dats5/atlas/SgTopD3PD/mc11c_v10/MC/ttbar"
  cp -r $dir /rdsk/dats5/atlas/SgTopD3PD/mc11c_v10/MC/ttbar
  l=`expr $l - 1`  
done

while test $m != $n
  do
  dir="$1.$m"
  echo "cp -r $dir /rdsk/dats4/atlas/SgTopD3PD/mc11c_v10/MC/ttbar"
  cp -r $dir /rdsk/dats4/atlas/SgTopD3PD/mc11c_v10/MC/ttbar
  m=`expr $m - 1`  
done

while test $n != $o
  do
  dir="$1.$n"
  echo "cp -r $dir /rdsk/dats3/atlas/SgTopD3PD/mc11c_v10/MC/ttbar"
  cp -r $dir /rdsk/dats3/atlas/SgTopD3PD/mc11c_v10/MC/ttbar
  n=`expr $n - 1`  
done

while test $o != $p
  do
  dir="$1.$o"
  echo "cp -r $dir /rdsk/dats2/atlas/SgTopD3PD/mc11c_v10/MC/ttbar"
  cp -r $dir /rdsk/dats2/atlas/SgTopD3PD/mc11c_v10/MC/ttbar
  o=`expr $o - 1`  
done

while test $p != $q
  do
  if [ $p -gt 9 ]; then
      dir="$1.$p" 
  else
      dir="$1.0$p" 
  fi
  echo "cp -r $dir /rdsk/dats1/atlas/SgTopD3PD/mc11c_v10/MC/ttbar"
  cp -r $dir /rdsk/dats1/atlas/SgTopD3PD/mc11c_v10/MC/ttbar
  p=`expr $p - 1`  
done

exit 0
