#!/bin/sh
vagrant ssh -c "cd /vagrant/code/build_vagrant && cmake .. -DCMAKE_BUILD_TYPE=Release -DBOOST_ROOT=/usr/local/boost_1_60_0/ -DHDF5_DIR=/usr/local/hdf5 -G Ninja `echo $1`"
