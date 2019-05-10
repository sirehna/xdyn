#!/bin/sh
docker run \
    --name xdyn-ninja-windows \
    --rm \
    -u $(id -u $USER):$(id -g $USER) \
    -v $(pwd):/opt/share \
    -w /opt/share \
    sirehna/base-image-win64-gcc540-win32threads-ssc-xdyn \
    /bin/bash -c "mkdir -p /opt/share/.wine && export WINEPREFIX=/opt/share/.wine && wine winecfg && cd build_win && ninja `echo $*` "
