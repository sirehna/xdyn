#!/bin/sh
cd fetch_gitlab_artifacts
docker build --build-arg UID=$(id -u) --build-arg GID=$(id -g) --build-arg USER=$(whoami) -t fetch_gitlab_artifacts .
docker run -i --rm -u $( id -u $USER ):$( id -g $USER ) -v $(pwd):/out --net=host -w /out fetch_gitlab_artifacts `echo $*`
mv artifacts.zip ..
cd ..
unzip artifacts.zip
rm artifacts.zip
