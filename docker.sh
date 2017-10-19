#!/bin/sh
docker run -i --rm -v /etc/group:/etc/group:ro -v /etc/passwd:/etc/passwd:ro -u $( id -u $USER ):$( id -g $USER ) -v $(pwd)/build:/build -w /build -v $(pwd):/opt/share build-xdyn `echo $*`
