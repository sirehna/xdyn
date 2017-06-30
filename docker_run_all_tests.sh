#!/bin/sh
docker run --rm -v $(pwd):/opt/share -w /opt/share/code/build -it xdyn ./run_all_tests `echo $*`
