#!/bin/sh
# Using --security-opt seccomp=unconfined to avoid GDB error: warning: Error disabling address space randomization: Operation not permitted
# As per https://stackoverflow.com/questions/35860527/warning-error-disabling-address-space-randomization-operation-not-permitted
docker build -t valgrind -f Dockerfile.valgrind .
docker run $TERMINAL \
    --security-opt seccomp=unconfined \
    --rm \
    -u $( id -u $USER ):$( id -g $USER ) \
    -v $(pwd)/build_deb10_dbg:/build \
    -w /build \
    -it \
    valgrind \
    /bin/bash -c "export LD_LIBRARY_PATH=/build; valgrind --leak-check=full --track-origins=yes --error-exitcode=1 ./run_all_tests `echo $*`"
