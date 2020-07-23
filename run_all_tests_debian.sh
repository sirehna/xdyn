#!/bin/sh
# Using --security-opt seccomp=unconfined to avoid GDB error: warning: Error disabling address space randomization: Operation not permitted
# As per https://stackoverflow.com/questions/35860527/warning-error-disabling-address-space-randomization-operation-not-permitted
docker run $TERMINAL \
    --security-opt seccomp=unconfined \
    --rm \
    -u $( id -u $USER ):$( id -g $USER ) \
    -v $(pwd)/build_deb10:/build \
    -w /build \
    -t \
    sirehna/base-image-debian10-gcc8-xdyn \
    /bin/bash -c "export LD_LIBRARY_PATH=/build; ./run_all_tests `echo $*`"
