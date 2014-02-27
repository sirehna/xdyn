#!/bin/bash
# Usage: foreach.sh list_of_dependencies.txt script

while read line
do
    echo "Executing $2 $line..."
    ./$2 $line $3 $4 $5
done < $1
