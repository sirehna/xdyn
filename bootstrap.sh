#!/usr/bin/env bash


# Default Debian URL is buggy when using Vagrant or Docker from France:
# Error reading from server. Remote end closed connection [IP: 128.31.9.66 80]
sudo sed -i 's|httpredir.debian.org|ftp.debian.org|g' /etc/apt/sources.list

# Update package list
sudo apt-get update

# LCOV
sudo apt-get install lcov -y

# G++
sudo apt-get install g++ -y

# GDB
sudo apt-get install gdb -y

# PANDOC
sudo apt-get install pandoc -y

# PYTHON3 + TORNADO
sudo apt-get install python3-tornado -y

# PIP3
sudo apt-get install python3-pip -y

# CXFREEZE
# As per http://stackoverflow.com/a/36593715
wget https://pypi.python.org/packages/62/ff/263fae9f69150c1b696a94dd23ca48c02512228c53bb108aeaccda192028/cx_Freeze-4.3.4.tar.gz#md5=5bd662af9aa36e5432e9144da51c6378
sudo apt-get install libssl-dev -y
tar xzf cx_Freeze-4.3.4.tar.gz
sed -i 's/if not vars.get("Py_ENABLE_SHARED", 0):/if True:/g'  cx_Freeze-4.3.4/setup.py
sudo pip3 install cx_Freeze-4.3.4/


# DOXYGEN
sudo apt-get install doxygen -y

# GFORTRAN
sudo apt-get install gfortran -y

# NINJA
sudo apt-get install ninja-build -y

# GIT
sudo apt-get install git -y

# SUBVERSION
sudo apt-get install subversion -y

# # BOOST 1.55
# svn co http://svn.boost.org/svn/boost/tags/release/Boost_1_55_0/
# svn co http://svn.boost.org/svn/boost/trunk/boost/geometry/extensions
# cd Boost_1_55_0
# ./bootstrap.sh
# sudo ./b2 cxxflags=-fPIC link=static link=shared threading=single threading=multi --layout=tagged --prefix=/usr/local/boost_1_55_0 install
# sudo cp -r /home/vagrant/extensions /usr/local/boost_1_55_0/include/boost/geometry/

# BOOST 1.60
BOOST_INSTALL=/usr/local/boost_1_60_0
wget http://sourceforge.net/projects/boost/files/boost/1.60.0/boost_1_60_0.tar.gz
tar -xf boost_1_60_0.tar.gz
cd boost_1_60_0
./bootstrap.sh
# link=shared threading=multi
sudo ./b2 cxxflags=-fPIC link=static threading=single --layout=tagged --prefix=${BOOST_INSTALL} install
sudo chown -R vagrant boost_1_60_0
sudo chgrp -R vagrant boost_1_60_0

# GMOCK
cd /vagrant/code
sudo rm -rf googletest
git clone https://github.com/google/googletest.git googletest
cd googletest
git checkout release-1.8.0
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
sudo chown -R vagrant yaml-cpp
sudo chgrp -R vagrant yaml-cpp
cd yaml-cpp
git checkout tags/release-0.3.0
cd ..
cp ../yaml-cpp-CMakeLists.txt yaml-cpp/CMakeLists.txt

# GCOVR
cd /vagrant/code
sudo rm -rf gcovr
git clone https://github.com/gcovr/gcovr.git
cd gcovr
git checkout tags/3.2
cd ..
sudo chown -R vagrant gcovr
sudo chgrp -R vagrant gcovr

# CMAKE
sudo apt-get install -y cmake
# wget https://cmake.org/files/v3.5/cmake-3.5.2.tar.gz
# tar xzf cmake-3.5.2.tar.gz
# cd cmake-3.5.2
# ./configure
# make
# sudo make install

# HDF5
cd ~
HDF5_INSTALL=/usr/local/hdf5
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
"-DCMAKE_INSTALL_PREFIX:PATH=${HDF5_INSTALL}" \
"-DCMAKE_C_FLAGS=-fpic" \
"-DCMAKE_CXX_FLAGS=-fpic"
sudo make install

# SSC (Sirehna Scientific Computing)
cd /vagrant
sudo dpkg -i ssc.deb

# FIRST BUILD
cd /vagrant/code
mkdir build_vagrant
cd build_vagrant
cmake .. -DCMAKE_BUILD_TYPE=Release -DBOOST_ROOT:PATH=${BOOST_INSTALL} -DHDF5_DIR:PATH=${HDF5_INSTALL} -G Ninja
ninja
cpack

