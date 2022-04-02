#!/bin/bash 

for FILE in $2/*.in; do 

OUT=cat "${FILE::-2}out"
ERROR=cat "${FILE::-2}err"

echo $OUT
echo $ERROR

done;