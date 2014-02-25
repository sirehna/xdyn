#!/bin/bash

function status {
    echo "Status of module '$2'"
    cd $2 && git status && cd ..
}

while read line
do
    status $line
done < $1
