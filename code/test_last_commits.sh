#!/bin/sh
for i in `git rev-list --max-count=$1 HEAD`; do .././test_build.sh $i; done
