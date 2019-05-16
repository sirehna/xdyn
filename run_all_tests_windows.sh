#!/bin/sh
docker run \
    --name run-all-tests-windows \
    --rm \
    -u $( id -u $USER ):$( id -g $USER ) \
    -v $(pwd)/build_win:/opt/share \
    -w /opt/share \
    sirehna/base-image-win64-gcc540-win32threads-ssc-xdyn \
    /bin/bash -c "mkdir -p /opt/share/.wine && export WINEPREFIX=/opt/share/.wine && wine winecfg && wine ./run_all_tests `echo $*`"
