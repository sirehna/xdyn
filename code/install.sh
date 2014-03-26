#!/bin/bash

if [ $# -eq 0 ]
then
    buildType=Debug
else
    buildType=$1
fi

if [[ `gcc -dumpmachine` == x86_64* ]]
then
    echo "Target 64 bits"
    suffix=64
else
    echo "Target 32 bits"
    suffix=32
fi

buildType="$(tr '[:lower:]' '[:upper:]' <<< ${buildType:0:1})$(tr '[:upper:]' '[:lower:]' <<< ${buildType:1})"

if [ `uname -s` = "Linux" ];
then
    cmakeGenerator="Unix Makefiles"
else
    cmakeGenerator="MSYS Makefiles"
fi

mkdir -p ../build${buildType}${suffix}
cd ../build${buildType}${suffix}
cmake ../code -G"${cmakeGenerator}" -DCMAKE_BUILD_TYPE=${buildType} -DINSTALL_PREFIX=../install${buildType}${suffix}
make install
make test

if [ ! `uname -s` = "Linux" ];
then
    make package
fi

if [ ${buildType} = "Coverage" ];
then
    make coverage
    make coverage-xml
fi
