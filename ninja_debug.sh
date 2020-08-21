#!/bin/sh
docker run \
    -t \
    --rm \
    -u $( id -u ):$( id -g ) \
    -v $(pwd)/build_deb10_dbg:/build \
    -w /build \
    -v $(pwd):/opt/share \
    sirehna/base-image-debian10-gcc8-xdyn \
    /bin/sh -c "cd /build && ninja `echo $*`"
