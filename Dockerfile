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

FROM ssc
MAINTAINER Guillaume Jacquenot <guillaume.jacquenot@sirehna.com>

# Install dependencies
# RUN apt-get update -yq && apt-get install -y \
#     build-essential \
#     wget \
#     git \
#     cmake \
#     ninja-build \
#     python3 \
#     python3-dev \
#     gfortran \
#     libblas3 \
#     liblapack3 \
#     libbz2-dev \
#     && apt-get clean \
#     && rm -rf /tmp/* /var/tmp/* \
#     && rm -rf /var/lib/apt/lists/

RUN mkdir -p /opt
WORKDIR /opt

ENV PATH="/opt/cmake/bin:${PATH}"

RUN mkdir -p /opt/share
ADD . /opt/share

RUN mkdir -p xdyn_build \
    && cd xdyn_build \
    && cmake -Wno-dev \
             -G Ninja \
             -DCMAKE_INSTALL_PREFIX:PATH=/opt/xdyn \
             ../share/code \
    && ninja \
    && ./run_all_tests --gtest_output=xml:run_all_tests.xml \
    && ninja package \
    && ninja install \
    && cd .. \
    && rm -rf xdyn_build

CMD bash
