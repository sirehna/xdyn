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
        libssl-dev && \
    apt-get clean && \
    rm -rf /tmp/* /var/tmp/* && \
    rm -rf /var/lib/apt/lists/

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
    # link=shared threading=multi
    ./b2 cxxflags=-fPIC link=static threading=single --layout=tagged --prefix=${BOOST_INSTALL} install || true && \
    cd .. \
    rm -rf boost_source.tar.gz boost_SRC


ENV HDF5_INSTALL="/usr/local/hdf5"
RUN wget https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-1.8/hdf5-1.8.12/src/hdf5-1.8.12.tar.gz -O hdf5_source.tar.gz
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

RUN rm -rf /opt/*

ADD ssc.deb /opt/share/ssc.deb

RUN dpkg -i /opt/share/ssc.deb
