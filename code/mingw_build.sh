#!/bin/sh
mkdir build
./foreach.sh externalDependencies.txt fetch_rebase.sh
cd internal_solver_validation
./foreach.sh externalDependencies.txt fetch_rebase.sh
cd build
cmake .. -G "MSYS Makefiles" -DCMAKE_COLOR_MAKEFILE=OFF -DCMAKE_BUILD_TYPE=Debug
cd ..
