#!/bin/sh
rm -rf user control.tar.gz data.tar.gz debian-binary usr
cp ../build/xdyn.deb ../spt-0.0.0-py2.py3-none-any.whl .
ar x xdyn.deb
tar xzf data.tar.gz
rm -rf user control.tar.gz data.tar.gz  debian-binary usr/demos usr/doc usr/matlab
docker build -t xdyn -f Dockerfile .
rm -rf usr
rm -rf xdyn.tar.gz || true
docker save xdyn | gzip -c > xdyn.tar.gz
