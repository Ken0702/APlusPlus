# give write and read permission to the group
# for /rdsk/datXY/atlas/SgTopD3PD/*

# i=8
# while test $i != 0
# do
#   cd /rdsk/dats$i/atlas
#   chmod -R g+wr SgTopD3PD
#   i=`expr $i - 1`
# done

# i=8
# while test $i != 4
# do
#   cd /rdsk/datm$i/atlas
#   chmod -R g+wr SgTopD3PD
#   i=`expr $i - 1`
# done
# exit 0

# shorter way:
#chmod 755 $(find /rdsk/dat[ms][1-8]/atlas/SgTopD3PD -type d -user $USER)
#chmod 644 $(find /rdsk/dat[ms][1-8]/atlas/SgTopD3PD -type f -user $USER) #argument list too long
find /rdsk/dat[msv][1-8]/atlas/SgTop* -type d -user $USER -print0 | xargs -0 chmod 775
find /rdsk/dat[msv][1-8]/atlas/SgTop* -type f -user $USER -print0 | xargs -0 chmod 664
