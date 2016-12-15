#!/bin/bash
filelist=`ls`
for file in $filelist
do 
    if [ "$file" = "rename.sh" ]; then
        continue
    else
        newname=`md5sum $file`
        md5=`echo $newname | awk '{print $1}'`
        suffix=`echo $newname | awk -F. '{print $2}'`
        # echo $md5
        # echo $suffix
        newname="$md5"".""$suffix"
        # echo $newname
        echo "mov $file to $newname"
        mv $file $newname
    fi
done
