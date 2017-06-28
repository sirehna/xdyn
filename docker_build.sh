#!/bin/sh

# Get SSC
cd ssc_getter
docker build -f Dockerfile -t get_ssc .
#docker run -it --rm -v $(pwd):/out --net=host get_ssc
docker run -it --rm -v /etc/group:/etc/group:ro  -v /etc/passwd:/etc/passwd:ro -u $( id -u $USER ):$( id -g $USER ) -v $(pwd):/out --net=host -w /out get_ssc python3 fetch_ssc.py
unzip artifacts.zip
rm artifacts.zip
mv ssc.deb ..
cd ..


mkdir -p build

# Build X-DYN container
docker build -f Dockerfile -t xdyn .

# Run CMake
docker run --rm -v $(pwd)/build:/build -w /build -v $(pwd):/opt/share -it xdyn cmake -Wno-dev \
    -G Ninja \
    -DINSTALL_PREFIX:PATH=/opt/xdyn \
    -Dssc_DIR:PATH=/opt/ssc/lib/ssc/cmake \
    -DHDF5_DIR:PATH=/usr/local/hdf5 \
    -DBOOST_ROOT:PATH=/usr/local/boost_1_60_0 \
    /opt/share/code 


# Build
docker run --rm -v $(pwd)/build:/build -w /build -v $(pwd):/opt/share -it xdyn ninja

# Run all tests
docker run --rm -v $(pwd)/build:/build -w /build -v $(pwd):/opt/share -it xdyn ./run_all_tests --gtest_output=xml:run_all_tests.xml 

