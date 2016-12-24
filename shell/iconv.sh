#!/bin/bash
filelist=`ls`
for file in $filelist
do 
    if [ "$file" = "iconv.sh" ]; then
        continue
    fi
    if [ -d $file ]; then
        echo "$file is a dir, do nothing"
        continue
    else
        newname="u_""$file"
        iconv -f GBK -t UTF-8 $file > $newname
        rm $file
    fi
done
