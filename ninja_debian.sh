#!/bin/sh
docker run \
    --name xdyn-ninja-debian \
    --rm \
    -u $( id -u $USER ):$( id -g $USER ) \
    -v $(pwd)/build_debian:/build \
    -w /build \
    -v $(pwd):/opt/share \
    sirehna/base-image-debian9-gcc6-xdyn \
    /bin/sh -c "cd /opt/share/doc_user/images && make -i > /dev/null 2> /dev/null && cd /build && ninja `echo $*`"
