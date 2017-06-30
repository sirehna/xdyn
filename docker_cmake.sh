#!/bin/sh
docker run --rm -v $(pwd):/opt/share -w /opt/share/code/build -it xdyn cmake `echo $*`
