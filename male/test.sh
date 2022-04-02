#!/bin/bash 

echo "starting tests"

for FILE in $2/*.in; do 

prefix="$2/"
suffix=".in"
name=${FILE#"$prefix"}
name=${name%"$suffix"}
echo "running test ${name}"

OUT=$($1 < $FILE )
ERROR=$($1 < $FILE 2>&1 1> /dev/null)
CORROUT=$(<"${FILE::-2}out")
CORRERROR=$(<"${FILE::-2}err")

if [ "$ERROR" == "$CORRERROR" -a "$CORROUT" == "$OUT" ]; 
then
    echo "test ${name} ok"
else
    echo "test ${name} failed"
fi

done;

echo "tests finished"