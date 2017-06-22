# Build image
#   docker build -f Dockerfile -t xdyn .
#
# Delete image
#   docker rmi xdyn
#
# Run container
#   docker run --name test_xdyn xdyn
#   docker stop test_xdyn
#   docker rm test_xdyn
#
# Run interactive
#   docker run -it xdyn /bin/bash
#
# It builds xdyn, tests it and installs it

# Require Internet to download all dependencies

FROM debian
MAINTAINER Charles-Edouard Cady <charles-edouard.cady@sirehna.com>

WORKDIR /opt

ENV PATH="/opt/cmake/bin:${PATH}"

ENV LD_LIBRARY_PATH=/opt/xdyn/lib:/opt/xdyn/bin

RUN apt-get update -yq && apt-get install -y \
    cmake ninja-build git subversion wget \
    lcov g++ gdb gfortran \
    pandoc python3-pandas python3-pip python3-matplotlib texlive-fonts-recommended texlive-latex-extra dvipng inkscape doxygen\
    python3-tornado\
    libssl-dev\
    && apt-get clean \
    && rm -rf /tmp/* /var/tmp/* \
    && rm -rf /var/lib/apt/lists/


# Install dependencies
# BOOST 1.60
ENV BOOST_INSTALL=/usr/local/boost_1_60_0
RUN wget http://sourceforge.net/projects/boost/files/boost/1.60.0/boost_1_60_0.tar.gz
RUN tar -xf boost_1_60_0.tar.gz
RUN cd boost_1_60_0 && ./bootstrap.sh
# link=shared threading=multi
RUN cd boost_1_60_0 && ./b2 cxxflags=-fPIC link=static threading=single --layout=tagged --prefix=${BOOST_INSTALL} install || true


ENV HDF5_INSTALL="/usr/local/hdf5"
RUN wget https://www.hdfgroup.org/ftp/HDF5/releases/hdf5-1.8.12/src/hdf5-1.8.12.tar.gz -O hdf5_source.tar.gz
RUN mkdir -p hdf5_source
RUN tar -xf hdf5_source.tar.gz --strip 1 -C ./hdf5_source
RUN mkdir hdf5_build \
 && cd hdf5_build \
 && cmake \
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
RUN cd hdf5_build && make install


RUN mkdir -p /opt/share
ADD . /opt/share
RUN dpkg -i /opt/share/ssc.deb

RUN cmake -Wno-dev \
          -G Ninja \
          -DINSTALL_PREFIX:PATH=/opt/xdyn \
          -Dssc_DIR:PATH=/opt/ssc/lib/ssc/cmake \
          -DHDF5_DIR:PATH=${HDF5_INSTALL} \
          -DBOOST_ROOT:PATH=${BOOST_INSTALL} \
          /opt/share/code 
RUN ninja 
RUN ./run_all_tests --gtest_output=xml:run_all_tests.xml 
#ENTRYPOINT /bin/bash
