#!/bin/sh
docker run \
    --name xdyn-ninja-debian \
    --rm \
    -u $( id -u ):$( id -g ) \
    -v $(pwd)/build_deb9:/build \
    -w /build \
    -v $(pwd):/opt/share \
    sirehna/base-image-debian9-gcc6-xdyn \
    /bin/sh -c "cd /build && ninja `echo $*`"
