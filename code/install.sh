#!/bin/bash

if [ $# -eq 0 ]
then
    buildType=Debug
else
    buildType=$1
fi

buildType="$(tr '[:lower:]' '[:upper:]' <<< ${buildType:0:1})$(tr '[:upper:]' '[:lower:]' <<< ${buildType:1})"

if [ `uname -s` = "Linux" ];
then
    cmakeGenerator="Unix Makefiles"
else
    cmakeGenerator="MSYS Makefiles"
fi

mkdir -p ../build${buildType}
cd ../build${buildType}
cmake ../code -G"${cmakeGenerator}" -DCMAKE_BUILD_TYPE=${buildType}
make install
make test

if [ ! `uname -s` = "Linux" ];
then
    make package
fi

