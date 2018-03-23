#!/bin/sh
docker run --name xdyn-ninja-debian --rm -v /etc/group:/etc/group:ro -v /etc/passwd:/etc/passwd:ro -u $( id -u $USER ):$( id -g $USER ) -v $(pwd)/build_debian:/build -w /build -v $(pwd):/opt/share -i build-xdyn-debian /bin/sh -c "cd /opt/share/doc_user/images && make -i > /dev/null 2> /dev/null && cd /build && ninja `echo $*`"
