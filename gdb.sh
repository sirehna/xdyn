#!/bin/sh
# Using --security-opt seccomp=unconfined to avoid GDB error: warning: Error disabling address space randomization: Operation not permitted
# As per https://stackoverflow.com/questions/35860527/warning-error-disabling-address-space-randomization-operation-not-permitted
docker build -t gdb gdb
docker run $TERMINAL \
    --security-opt seccomp=unconfined \
    --rm \
    -u $(id -u):$(id -g) \
    -v $(pwd)/build_deb10_dbg:/build \
    -v $(pwd)/code:/opt/share/code/ \
    -w /build \
    -it \
    gdb \
    "`echo $*`"
