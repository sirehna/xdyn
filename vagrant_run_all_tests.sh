#!/bin/sh
vagrant ssh -c 'cd /vagrant/code/build_vagrant && ./run_all_tests --gtest_filter=-*LONG*'
