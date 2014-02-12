#!/bin/bash

if [ `uname -s` = "Linux" ];
then
    cmakeGenerator="Unix Makefiles"
else
    cmakeGenerator="MSYS Makefiles"
fi

mkdir -p ../build
cd ../build
cmake ../code -G"${cmakeGenerator}" -DCMAKE_BUILD_TYPE=Release
make install
make test

if [ ! `uname -s` = "Linux" ];
then
    make package
fi

