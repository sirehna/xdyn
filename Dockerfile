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

FROM debian:8
MAINTAINER Charles-Edouard Cady <charles-edouard.cady@sirehna.com>

WORKDIR /opt

ENV PATH="/opt/cmake/bin:${PATH}"

ENV LD_LIBRARY_PATH=/opt/xdyn/lib:/opt/xdyn/bin

RUN apt-get update -yq && apt-get install --no-install-recommends -y \
        cmake make ninja-build git subversion wget \
        g++-4.9 \
        gcc-4.9 \
        gfortran-4.9 \
        lcov gdb \
        python3-pandas python3-pip python3-matplotlib python3-tornado \
        pandoc \
        texlive-fonts-recommended texlive-latex-extra lmodern \
        inkscape doxygen dvipng \
        libssl-dev  \
        pkg-config \
        autoconf \
        automake \
        libtool \
        curl \
        unzip \
 && apt-get clean \
 && rm -rf /tmp/* /var/tmp/* \
 && rm -rf /var/lib/apt/lists/

RUN update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.9 100
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.9 100
RUN update-alternatives --set g++ /usr/bin/g++-4.9
RUN update-alternatives --set gcc /usr/bin/gcc-4.*

# Install dependencies
# BOOST 1.60
ENV BOOST_INSTALL=/usr/local/boost_1_60_0
RUN wget http://sourceforge.net/projects/boost/files/boost/1.60.0/boost_1_60_0.tar.gz -O boost_source.tar.gz && \
    mkdir -p boost_SRC && \
    tar -xf boost_source.tar.gz --strip 1 -C boost_SRC && \
    cd boost_SRC && \
    ./bootstrap.sh && \
    ./b2 cxxflags=-fPIC link=static threading=single --layout=tagged --prefix=${BOOST_INSTALL} install || true && \
    cd .. \
    rm -rf boost_source.tar.gz boost_SRC


ENV HDF5_INSTALL="/usr/local/hdf5"
RUN wget https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-1.8/hdf5-1.8.12/src/hdf5-1.8.12.tar.gz -O hdf5_source.tar.gz && \
    mkdir -p HDF5_SRC && \
    tar -xf hdf5_source.tar.gz --strip 1 -C HDF5_SRC && \
    mkdir -p HDF5_build && \
    cd HDF5_build && \
    cmake -G"Unix Makefiles" \
      -DCMAKE_BUILD_TYPE:STRING=Release \
      -DCMAKE_INSTALL_PREFIX:PATH=${HDF5_INSTALL} \
      -DBUILD_SHARED_LIBS:BOOL=OFF \
      -DBUILD_TESTING:BOOL=OFF \
      -DHDF5_BUILD_TOOLS:BOOL=OFF \
      -DHDF5_BUILD_EXAMPLES:BOOL=OFF \
      -DHDF5_BUILD_HL_LIB:BOOL=ON \
      -DHDF5_BUILD_CPP_LIB:BOOL=ON \
      -DHDF5_BUILD_FORTRAN:BOOL=OFF \
      -DCMAKE_C_FLAGS="-fPIC" \
      -DCMAKE_CXX_FLAGS="-fPIC" \
      ../HDF5_SRC && \
    make install && \
    cd .. && \
    rm -rf hdf5_source.tar.gz HDF5_SRC HDF5_build

RUN git clone https://github.com/google/protobuf.git
RUN cd protobuf \
 && git checkout  3.0.x \
 && ./autogen.sh \
 && ./configure "CFLAGS=-fPIC" "CXXFLAGS=-fPIC" \
 && make -j 4\
 && make check \
 && make install \
 && ls /usr/local/lib/libprotobuf.a
RUN ldconfig

RUN git clone https://github.com/zeromq/libzmq.git

RUN cd libzmq \
 && git checkout v4.2.2 \
 && mkdir build \
 && cd build \
 && cmake -DWITH_PERF_TOOL=OFF -DZMQ_BUILD_TESTS=ON -DENABLE_CPACK=OFF -DCMAKE_BUILD_TYPE=Release .. \
      -DCMAKE_C_FLAGS="-fPIC" \
      -DCMAKE_CXX_FLAGS="-fPIC" \
 && make -j 4 \
 && echo "if(NOT TARGET libzmq) # in case find_package is called multiple times" >> ZeroMQConfig.cmake \
 && echo "  add_library(libzmq SHARED IMPORTED)" >> ZeroMQConfig.cmake \
 && echo "  set_target_properties(libzmq PROPERTIES IMPORTED_LOCATION \${\${PN}_LIBRARY})" >> ZeroMQConfig.cmake \
 && echo "endif(NOT TARGET libzmq)" >> ZeroMQConfig.cmake \
 && echo "" >> ZeroMQConfig.cmake \
 && echo "if(NOT TARGET libzmq-static) # in case find_package is called multiple times" >> ZeroMQConfig.cmake \
 && echo "  add_library(libzmq-static STATIC IMPORTED)" >> ZeroMQConfig.cmake \
 && echo "  set_target_properties(libzmq-static PROPERTIES IMPORTED_LOCATION \${\${PN}_STATIC_LIBRARY})" >> ZeroMQConfig.cmake \
 && echo "endif(NOT TARGET libzmq-static)" >> ZeroMQConfig.cmake \
 && make test \
 && make install \
 && ldconfig \
 && ls /usr/share/cmake-3.0/Modules/

RUN git clone https://github.com/zeromq/cppzmq.git
RUN cd cppzmq \
 && git checkout v4.2.2 \
 && mkdir build \
 && cd build \
 && cmake .. \
      -DCMAKE_C_FLAGS="-fPIC" \
      -DCMAKE_CXX_FLAGS="-fPIC" \
 && make -j 4 install

ARG CACHEBUST=1
RUN rm -rf /opt/share/ssc.deb
ADD ssc.deb /opt/share/ssc.deb
RUN dpkg -r ssc
RUN dpkg -i /opt/share/ssc.deb
