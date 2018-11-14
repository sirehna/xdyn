#!/bin/sh
docker run --name run-all-tests-windows --rm -u $( id -u $USER ):$( id -g $USER ) -v $(pwd):/opt/share -w /opt/share mydockcross/windows-x64 /bin/bash -c "mkdir -p /opt/share/.wine && export WINEPREFIX=/opt/share/.wine && wine winecfg && cd build_windows && wine ./run_all_tests `echo $*`"
