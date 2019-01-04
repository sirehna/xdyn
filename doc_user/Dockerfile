# Dockerfile with xdyn, a python interpreter, various python libraries, and
# pandoc
#
FROM python:3.7.2-slim-stretch

# Install gcc to build psutil package used by pandoc-eqnos
RUN apt-get update && \
    apt-get install --yes gcc && \
    python3 -m pip install psutil && \
    apt-get remove -y gcc

RUN python3 -m pip install \
        h5py \
        numpy \
        matplotlib \
        pandas \
        pandoc-eqnos \
        pandoc-fignos \
        pweave \
        pyyaml \
        xlrd \
        xlwt \
        && \
    mkdir pandoc_bin && \
    cd pandoc_bin && \
    python3 -c 'from urllib.request import urlretrieve; urlretrieve("https://github.com/jgm/pandoc/releases/download/2.5/pandoc-2.5-linux.tar.gz", filename="pandoc.tar.gz")' && \
    tar -xzf pandoc.tar.gz --strip 1 && \
    ls && \
    cp bin/* /usr/bin && \
    cd .. && \
    rm -rf pandoc_bin pandoc.tar.gz
ADD report /
RUN cd /report && python3 setup.py install
ADD xdyn.deb /
RUN dpkg -r xdyn && dpkg -i xdyn.deb && \
    ln -s /usr/lib/libgfortran.so.3.0.0 /lib/x86_64-linux-gnu/libgfortran.so.3 && \
    ln -s /usr/lib/libquadmath.so.0.0.0 /lib/x86_64-linux-gnu/libquadmath.so.0
ENTRYPOINT ["pweave"]
