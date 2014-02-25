#!/bin/bash
# Usage: foreach.sh list_of_dependencies.txt script

while read line
do
    echo "Executing $2 $line..."
    ./$2 $line
done < $1
