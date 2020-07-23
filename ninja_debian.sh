#!/bin/sh
docker run \
    --name xdyn-ninja-debian \
    --rm \
    -u $( id -u ):$( id -g ) \
    -v $(pwd)/build_deb10:/build \
    -w /build \
    -v $(pwd):/opt/share \
    sirehna/base-image-debian10-gcc8-xdyn \
    /bin/sh -c "cd /build && ninja `echo $*`"
