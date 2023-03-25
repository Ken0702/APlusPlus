# Set "latest" link in
# /rdsk/datXY/atlas/SgTopD3PD/
# to $1

if [ "$1" = "-h" ]; then
    echo "provide directory name as \$1, which will be linked to \"latest\" in /rdsk/datXY/atlas/SgTopD3PD/ on dats1-dats8 and datm5-datm8" 
    exit 0
fi


i=8
while test $i != 0
do
  cd /rdsk/dats$i/atlas/SgTopD3PD
  rm latest
  ln -s $1 latest
  i=`expr $i - 1`
done

i=8
while test $i != 4
do
  cd /rdsk/datm$i/atlas/SgTopD3PD
  rm latest
  ln -s $1 latest
  i=`expr $i - 1`
done
exit 0
