#!/bin/sh
docker run --name xdyn-ninja-windows --rm -v /etc/group:/etc/group:ro  -v /etc/passwd:/etc/passwd:ro -u $(id -u $USER):$(id -g $USER) -v $(pwd):/opt/share -w /opt/share mydockcross/windows-x64 /bin/bash -c "mkdir -p /opt/share/.wine && export WINEPREFIX=/opt/share/.wine && wine winecfg && cd build_windows && ninja `echo $*` "
