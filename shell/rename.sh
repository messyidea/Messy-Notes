#!/bin/bash
filelist=`ls`
for file in $filelist
do 
    if [ "$file" = "rename.sh" ]; then
        continue
    fi
    if [ -d $file ]; then
        echo "$file is a dir, do nothing"
        continue
    else
        newname=`md5sum $file`
        md5=`echo $newname | awk '{print $1}'`
        suffix=`echo $newname | awk -F. '{print $2}'`
        # echo $md5
        # echo $suffix
        newname="$md5"".""$suffix"
        # echo $newname
        if [ "$file" = "$newname" ]; then
            echo "$file do not need to change name"
            continue
        else
            echo "mov $file to $newname"
            mv $file $newname
        fi
    fi
done
