#!/usr/bin/env python
# coding=utf-8
import sys
import os
import string
import codecs

files = os.listdir('.')
for filename in files:
    if os.path.isdir(filename):
        continue
    try:
        tokens = string.splitfields(filename, '.')
        if len(tokens) != 2 or tokens[1] != 'txt':
            continue
        else:
            print filename
            filedata = open(filename)
            tstr = filedata.read()
            tstr = tstr.decode("GBK")
            tstr = tstr.encode("UTF-8")
            filedata.close()
            filedata = open(filename, 'w')
            filedata.write(tstr)
            filedata.close()
    except:
        print "error on %s" %filename
