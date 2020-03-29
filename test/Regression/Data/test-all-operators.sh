#!/bin/sh

if [ "$1" != "" ]; then

if ! xcompiler -q $1 ; then exit ; fi

xretractor -m 8000 &

sleep 5
xqry -d
xqry -s str3 -m 5 
xqry -s str2 -m 5
xqry -l
xqry -k

else
    echo "missing source file. Try query-all.txt"
fi
