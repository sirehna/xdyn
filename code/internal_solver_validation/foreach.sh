#!/bin/bash
# Usage: foreach.sh list_of_dependencies.txt script

while read line
do
    echo "Executing $2 $3 $4 $5 $line..."
    ./$2 $3 $4 $5 $line
done < $1
