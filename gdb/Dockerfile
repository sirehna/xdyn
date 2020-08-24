#FROM xdyn
FROM sirehna/base-image-debian10-gcc8-xdyn
RUN apt-get update -yq && \
    apt-get install --no-install-recommends -y \
    gdb
ENV LD_LIBRARY_PATH=/build
ENTRYPOINT ["/usr/bin/gdb"]
