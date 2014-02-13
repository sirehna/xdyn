#!/bin/bash

function svnExport {
    echo "Retrieving $2"
    svn export $1/$2 $2
}

function svnExportWithVersionNumber {
    echo "Retrieving $2"
    svn export -r $3 $1/$2 $2
}

function svn_export {
    if [ $# -eq 0 ]
    then
        echo No argument entered for function 
        return
    fi
    if [ $# -eq 2 ]
    then
        svnExport $1 $2
    elif [ $# -eq 3 ]
    then
        svnExportWithVersionNumber $1 $2 $3
    fi
}

while read line
do
    echo "Dealing dependencies - $line"
    svn_export $line
done < $1
