#!/bin/bash

function svnExport {
    echo "> Retrieving $4"
    svn export --force --username $1 --password $2 --non-interactive $3/$4 $4
}

function svnExportWithVersionNumber {
    echo "> Retrieving '$4' version $5"
    svn export --force --username $1 --password $2 --non-interactive $3/$4/$5 $4
}

function svn_export {
    if [ $# -lt 4 ]
    then
        echo "Need at least four arguments: a username, a password, a repository & a destination folder. Optionally, a revision number as fifth argument."
        return
    fi
    if [ $# -eq 4 ]
    then
        svnExport $1 $2 $3 $4
    elif [ $# -eq 5 ]
    then
        svnExportWithVersionNumber $1 $2 $3 $4 $5
    else
        echo "Too many arguments: expected: a username, a password, a repository, a destination folder & (optionally) a revision number"
        return
    fi
}

svn_export $1 $2 $3 $4 $5
