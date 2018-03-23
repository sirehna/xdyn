#!/bin/sh

# Get SSC
cd fetch_gitlab_artifacts
docker build --build-arg UID=$(id -u) --build-arg GID=$(id -g) --build-arg USER=$(whoami) -f Dockerfile -t fetch_gitlab_artifacts .
docker run -i --rm -e USER=$(whoami) -e UID=$(id -u) -e GID=$(id -g)  -u $( id -u $USER ):$( id -g $USER ) -v $(pwd):/out --net=host -w /out fetch_gitlab_artifacts -c e3491f5ad68a11ac0414e496871429f74aacc493 --project_id 42 -b debian
unzip artifacts.zip
rm artifacts.zip
mv ssc.deb ..
cd ..

# Get SPT
cd fetch_gitlab_artifacts
docker run -i --rm -e USER=$(whoami) -e UID=$(id -u) -e GID=$(id -g)  -u $( id -u $USER ):$( id -g $USER ) -v $(pwd):/out --net=host -w /out fetch_gitlab_artifacts -c 44828c3a961044794426d0f4a56bd46600a1 --project_id 4
unzip artifacts.zip
rm artifacts.zip
rm -rf html
mv spt-0.0.0-py2.py3-none-any.whl ../deploy_xdyn
cd ..



mkdir -p code/build

# Build X-DYN container
docker build -f Dockerfile -t build-xdyn .

# Run CMake
sh docker_cmake.sh -Wno-dev \
                   -G Ninja \
                   -DINSTALL_PREFIX:PATH=/opt/xdyn \
                   -Dssc_DIR:PATH=/opt/ssc/lib/ssc/cmake \
                   -DHDF5_DIR:PATH=/usr/local/hdf5 \
                   -DBOOST_ROOT:PATH=/usr/local/boost_1_60_0 \
                   -DProtobuf_LIBRARY=/usr/local/lib/libprotobuf.a \
                   ..


# Build
sh docker_ninja.sh package

# Build X-DYN container
cd make_docker_image
./make_docker_image.sh
cd ..

# Run all tests
sh docker_run_all_tests.sh --gtest_output=xml:run_all_tests.xml
