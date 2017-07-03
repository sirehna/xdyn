#!/bin/sh
rm -rf user control.tar.gz  data.tar.gz  debian-binary usr
ar x xdyn.deb
tar xzf data.tar.gz
rm -rf user control.tar.gz  data.tar.gz  debian-binary usr/demos usr/doc usr/matlab
docker build -t xdyn -f Dockerfile .
rm -rf usr
docker save xdyn > xdyn.tar
