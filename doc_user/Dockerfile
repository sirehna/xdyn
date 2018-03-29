FROM python:3.6.4-slim-jessie
RUN apt-get update -yq && \
    apt-get install --yes --no-install-recommends wget python3-numpy python3-matplotlib && \
    apt-get clean && \
    rm -rf /tmp/* /var/tmp/* && \
    rm -rf /var/lib/apt/lists/ && \
    python3 -m pip install pweave
RUN wget https://github.com/jgm/pandoc/releases/download/2.1.3/pandoc-2.1.3-linux.tar.gz -O pandoc.tar.gz && \
    mkdir pandoc_bin && \
    cd pandoc_bin && \
    tar -xzf ../pandoc.tar.gz --strip 1 && \
    ls && \
    cp bin/* /usr/bin && \
    cd .. && \
    rm -rf pandoc_bin pandoc.tar.gz
ADD xdyn.deb /
RUN dpkg -r xdyn && dpkg -i xdyn.deb
ENTRYPOINT ["pweave"]
