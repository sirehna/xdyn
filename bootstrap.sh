#!/usr/bin/env bash


# Default Debian URL is buggy when using Vagrant or Docker from France:
# Error reading from server. Remote end closed connection [IP: 128.31.9.66 80]
sudo sed -i 's|httpredir.debian.org|ftp.debian.org|g' /etc/apt/sources.list

# Update package list
sudo apt-get update

# CMAKE
sudo apt-get install cmake -y

# G++
sudo apt-get install g++ -y

# GFORTRAN
sudo apt-get install gfortran -y

# NINJA
sudo apt-get install ninja-build -y

# GIT
sudo apt-get install git -y

# SUBVERSION
sudo apt-get install subversion -y

# BOOST 1.55
svn co http://svn.boost.org/svn/boost/tags/release/Boost_1_55_0/
svn co http://svn.boost.org/svn/boost/trunk/boost/geometry/extensions
cd Boost_1_55_0
./bootstrap.sh
sudo ./b2 cxxflags=-fPIC link=static link=shared threading=single threading=multi --layout=tagged --prefix=/usr/local/boost_1_55_0 install

# GMOCK
cd /vagrant/code
sudo rm -rf gmock
git clone https://github.com/google/googletest.git gmock
cd gmock
git checkout tags/release-1.7.0
cd ..
sudo chown -R vagrant gmock
sudo chgrp -R vagrant gmock

# EIGEN
cd /vagrant/code
wget http://bitbucket.org/eigen/eigen/get/3.2.0.tar.bz2 -O eigen.tar.bz2
sudo rm -rf eigen
tar xjf eigen.tar.bz2
mv eigen-eigen-ffa86ffb5570 eigen
sudo chown -R vagrant eigen
sudo chgrp -R vagrant eigen

# EIGEN3-HDF5
cd /vagrant/code
sudo rm -rf eigen3-hdf5
git clone https://github.com/garrison/eigen3-hdf5
sudo chown -R vagrant eigen3-hdf5
sudo chgrp -R vagrant eigen3-hdf5

# YAML-CPP
cd /vagrant/code
sudo rm -rf yaml-cpp
git clone https://github.com/jbeder/yaml-cpp.git
cd yaml-cpp
git checkout tags/release-0.3.0
cd ..
sudo chown -R vagrant yaml-cpp
sudo chgrp -R vagrant yaml-cpp

# GCOVR
cd /vagrant/code
sudo rm -rf gcovr
git clone https://github.com/gcovr/gcovr.git
cd gcovr
git checkout tags/3.2
cd ..
sudo chown -R vagrant gcovr
sudo chgrp -R vagrant gcovr


# HDF5
wget https://www.hdfgroup.org/ftp/HDF5/releases/hdf5-1.8.12/src/hdf5-1.8.12.tar.gz -O hdf5_source.tar.gz
mkdir -p hdf5_source
tar -xf hdf5_source.tar.gz --strip 1 -C ./hdf5_source
mkdir hdf5_build
cd hdf5_build
cmake \
-Wno-dev \
../hdf5_source \
"-DBUILD_SHARED_LIBS:BOOL=OFF" \
"-DCMAKE_BUILD_TYPE:STRING=Release" \
"-DHDF5_BUILD_HL_LIB:BOOL=ON" \
"-DHDF5_BUILD_FORTRAN:BOOL=ON" \
"-DHDF5_ENABLE_F2003:BOOL=ON" \
"-DHDF5_BUILD_CPP_LIB:BOOL=ON" \
"-DHDF5_BUILD_TOOLS:BOOL=ON" \
"-DCMAKE_INSTALL_PREFIX:PATH=/usr/local/hdf5" \
"-DCMAKE_C_FLAGS=-fpic" \
"-DCMAKE_CXX_FLAGS=-fpic"
sudo make install


# SSC
cd /vagrant
sudo dpkg -i ssc.deb

# FIRST BUILD
cd /vagrant/code
mkdir build
cd build
cmake .. -DBOOST_ROOT=/usr/local/boost_1_55_0 -DHDF5_DIR=/usr/local/hdf5
