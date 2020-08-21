#!/bin/sh
docker run  --rm \
    -u $(id -u):$(id -g) \
    -v $(pwd):/opt/share \
    -w /opt/share \
    sirehna/base-image-win64-gccx-posixthreads-ssc-xdyn /bin/bash -c \
       "cd /opt/share &&\
        mkdir -p build_win_posix &&\
        cd build_win_posix &&\
        mkdir -p /opt/share/.wine;\
        export WINEPREFIX=/opt/share/.wine;\
        wine winecfg;\
        ninja `echo $*` "
