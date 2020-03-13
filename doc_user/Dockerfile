# Dockerfile with xdyn, a python interpreter, various python libraries, and
# pandoc
#
FROM python:3.7.3-slim-stretch

# Install
# - gcc to build psutil package used by pandoc-eqnos
# - libgfortran3 as a dependency for xdyn
RUN apt-get update && \
    apt-get install --yes \
        gcc \
        libgfortran3 \
        libquadmath0 && \
    python3 -m pip install psutil && \
    apt-get remove -y gcc && \
    apt-get autoremove -y && \
    echo "Install pandoc" && \
    mkdir pandoc_bin && \
    cd pandoc_bin && \
    python3 -c 'from urllib.request import urlretrieve; urlretrieve("https://github.com/jgm/pandoc/releases/download/2.7.2/pandoc-2.7.2-linux.tar.gz", filename="pandoc.tar.gz")' && \
    tar -xzf pandoc.tar.gz --strip 1 && \
    ls && \
    cp bin/* /usr/bin && \
    cd .. && \
    rm -rf pandoc_bin pandoc.tar.gz

RUN python3 -m pip install -U pip && \
    python3 -m pip install \
        h5py==2.10.0 \
        jupyter_client==5.3.4 \
        matplotlib==3.2.0 \
        numpy==1.18.1 \
        pandas==1.0.1 \
        pandoc-eqnos==2.1.1 \
        pandoc-fignos==2.2.0 \
        pweave==0.30.1 \
        pyyaml==5.3 \
        xlrd==1.2.0 \
        xlwt==1.3.0 \
        && \
    mkdir -p /tmp_build/report

ADD setup.cfg setup.py /tmp_build/
ADD report /tmp_build/report
ADD xdyn.deb /
RUN cd /tmp_build && \
    python3 setup.py install && \
    cd / && \
    dpkg -r xdyn && \
    dpkg -i xdyn.deb && \
    xdyn --help && \
    mkdir -p /testdir && \
    chmod a+rwx /testdir && \
    echo "#!/bin/bash" > /usr/bin/xdyn-weave && \
    echo "rm -rf /work/tuto_execution" >> /usr/bin/xdyn-weave && \
    echo "cp -r /usr/demos /work/tuto_execution" >> /usr/bin/xdyn-weave && \
    echo "pweave \$*" >> /usr/bin/xdyn-weave && \
    chmod a+x /usr/bin/xdyn-weave
ENTRYPOINT ["/usr/bin/xdyn-weave"]
