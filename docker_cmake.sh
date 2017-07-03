#!/bin/sh
docker run --rm -v $(pwd):/opt/share -w /opt/share/code/build -it build-xdyn cmake `echo $*`
