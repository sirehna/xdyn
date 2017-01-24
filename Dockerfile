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
RUN apt-get update -yq && \
    apt-get install -y \
        python3-tornado \
        python3-pip \
        python3-pandas \
        libssl-dev && \
    apt-get clean && \
    rm -rf /tmp/* /var/tmp/* && \
    rm -rf /var/lib/apt/lists && \
    # CX_FREEZE
    wget https://pypi.python.org/packages/62/ff/263fae9f69150c1b696a94dd23ca48c02512228c53bb108aeaccda192028/cx_Freeze-4.3.4.tar.gz#md5=5bd662af9aa36e5432e9144da51c6378 -O cx_freeze_src.tar.gz && \
    mkdir -p cx_freeze_src && \
    tar xzf cx_freeze_src.tar.gz --strip 1 -C cx_freeze_src && \
    sed -i 's/if not vars.get("Py_ENABLE_SHARED", 0):/if True:/g'  cx_freeze_src/setup.py && \
    cd cx_freeze_src && \
    python3 setup.py build && \
    python3 setup.py install && \
    cd .. && \
    rm -rf cx_freeze_src && \
    rm -rf cx_freeze_src.tar.gz

RUN mkdir -p /opt
WORKDIR /opt

ENV PATH="/opt/cmake/bin:${PATH}"

ENV LD_LIBRARY_PATH=/opt/xdyn/lib:/opt/xdyn/bin

RUN mkdir -p /opt/share
ADD . /opt/share

RUN rm -rf /opt/xdyn \
    && mkdir -p /opt/xdyn \
    && mkdir -p xdyn_build \
    && cd xdyn_build \
    && cmake -Wno-dev \
             -G Ninja \
             -DINSTALL_PREFIX:PATH=/opt/xdyn \
             -Dssc_DIR:PATH=/opt/ssc/lib/ssc/cmake \
             -DHDF5_DIR:PATH=/opt/HDF5/share/cmake \
             -DBOOST_ROOT:PATH=/opt/boost \
             ../share/code \
    && ninja \
    && ./run_all_tests --gtest_output=xml:run_all_tests.xml \
    && ninja package \
    && ninja install \
    && cd .. \
    && rm -rf xdyn_build

CMD bash
