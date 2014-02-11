#!/bin/bash

function fetch {
    git svn init $1/$2 $2
    cd $2 && git svn fetch && cd ..
}

function fetchWithVersionNumber {
    git svn init $1/$2/$3 $2
    cd $2 && git svn fetch && cd ..
}

function fetchAndUpdate {
    if [ $# -eq 0 ]
    then
        echo No argument entered for function fetchAndUpdate
        echo Two or three arguments are required
        echo Prototype
        echo - fetchAndUpdate url_of_the_repository directory_to_fetch
        echo - fetchAndUpdate url_of_the_repository directory_to_fetch tag_number
        echo Example
        echo - fetchAndUpdate http://130.66.124.6/svn/maroff/Product/Cpp/Tools random_data_generator
        echo - fetchAndUpdate http://130.66.124.6/svn/tools/ThirdParty gmock 1.7.0
        return
    fi
    if [ ! -d $2 ]
    then
        if [ $# -eq 2 ]
        then
            fetch $1 $2
        elif [ $# -eq 3 ]
        then
            fetchWithVersionNumber $1 $2 $3
        fi
    else
        cd $2 && git svn rebase && cd ..
    fi
}

fetchAndUpdate http://130.66.124.6/svn/eonav/oad_and_monitoring_modules/trunk/libraries boost
fetchAndUpdate http://130.66.124.6/svn/eonav/oad_and_monitoring_modules/trunk/modules exception_handling
fetchAndUpdate http://130.66.124.6/svn/maroff/Product/Cpp/Tools random_data_generator
fetchAndUpdate http://130.66.124.6/svn/maroff/Product/Cpp/Tools macros
fetchAndUpdate http://130.66.124.6/svn/maroff/Product/Cpp/Tools interpolation
fetchAndUpdate http://130.66.124.6/svn/dev_pilotage/simulator_sdk/trunk data_source
fetchAndUpdate http://130.66.124.6/svn/dev_pilotage/simulator_sdk/trunk simulation
fetchAndUpdate http://130.66.124.6/svn/tools/ThirdParty gmock 1.7.0
fetchAndUpdate http://130.66.124.6/svn/tools/ThirdParty eigen 3.2.0
fetchAndUpdate http://130.66.124.6/svn/tools/ThirdParty lapack 3.5.0
