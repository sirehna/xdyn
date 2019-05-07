# docker build . -f Dockerfile_xdyn_min --tag xdyn_min
# docker run xdyn_min --help
FROM debian:stretch-slim
RUN apt-get update && \
    apt-get install --yes \
        libgfortran3 \
        libquadmath0
ADD xdyn.deb /
RUN dpkg -r xdyn && \
    dpkg -i xdyn.deb
ENTRYPOINT ["/usr/bin/xdyn"]
