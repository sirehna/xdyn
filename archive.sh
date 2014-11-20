#!/bin/bash
# Script used to create an archive containing all the source of the project
# with its external dependencies
currentWorkingDir=`pwd`
archiveDir=`basename ${currentWorkingDir}`
mkdir -p ${archiveDir}
git archive --format=tar HEAD | (cd ${archiveDir} && tar xf -)
cd ${archiveDir}
cd code
./foreach.sh externalDependencies.txt svn_export.sh jenkins KnsJn2705
cd ..
cd ..
rm -rf ${archiveDir}_`git rev-parse HEAD`.tgz
cd ${archiveDir}
tar -czf ${archiveDir}_`git rev-parse HEAD`.tgz code postprocessing doc_user doc_dev
mv *.tgz ..
cd ..
rm -rf ${archiveDir}
