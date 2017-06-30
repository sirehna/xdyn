#!/bin/sh

## Get SSC
#cd ssc_getter
#docker build -f Dockerfile -t get_ssc .
#docker run -it --rm -v /etc/group:/etc/group:ro  -v /etc/passwd:/etc/passwd:ro -u $( id -u $USER ):$( id -g $USER ) -v $(pwd):/out --net=host -w /out --net=host get_ssc python3 fetch_ssc.py
#unzip artifacts.zip
#rm artifacts.zip
#mv ssc.deb ..
#cd ..
#
#
#mkdir -p code/build
#
## Build X-DYN container
#docker build -f Dockerfile -t xdyn .

# Run CMake
sh docker_cmake.sh -Wno-dev \
                   -G Ninja \
                   -DINSTALL_PREFIX:PATH=/opt/xdyn \
                   -Dssc_DIR:PATH=/opt/ssc/lib/ssc/cmake \
                   -DHDF5_DIR:PATH=/usr/local/hdf5 \
                   -DBOOST_ROOT:PATH=/usr/local/boost_1_60_0 \
                   ..


# Build
sh docker_ninja.sh

# Run all tests
sh docker_run_all_tests.sh --gtest_output=xml:run_all_tests.xml 

