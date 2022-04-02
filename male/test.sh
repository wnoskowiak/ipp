#!/bin/bash 

echo "starting tests"
echo ""
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m'

for FILE in $2/*.in; do 

prefix="$2/"
suffix=".in"
name=${FILE#"$prefix"}
name=${name%"$suffix"}
echo -e "running test ${BLUE} ${name} ${NC}"

OUT=$($1 < $FILE )
ERROR=$($1 < $FILE 2>&1 1> /dev/null)
CORROUT=$(<"${FILE::-2}out")
CORRERROR=$(<"${FILE::-2}err")

if [ "$ERROR" == "$CORRERROR" -a "$CORROUT" == "$OUT" ]; 
then
    echo -e "${GREEN} test ${name} ok ${NC}"
else
    echo -e "${RED} test ${name} failed ${NC}"
fi

done;

echo "tests finished"