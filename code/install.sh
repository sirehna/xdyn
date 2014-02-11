#!/bin/bash

if [ `uname -s` = "Linux" ];
then
    cmakeGenerator="Unix Makefiles"
else
    cmakeGenerator="MSYS Makefiles"
fi

mkdir -p ../build
cd ../build
cmake ../code -G"${cmakeGenerator}"
make install
make test

