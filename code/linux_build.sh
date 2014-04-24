#!/bin/sh
mkdir build
./foreach.sh externalDependencies.txt svn_export.sh $1 $2
cd internal_solver_validation
./foreach.sh externalDependencies.txt svn_export.sh $1 $2
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cd ..
