#!/bin/sh
old_branch=`git symbolic-ref HEAD 2>/dev/null | cut -d"/" -f 3` #`git status | grep "# On branch" | sed 's/# On branch //g'`
git checkout $1
ninja -j 3
./run_all_tests
git checkout ${old_branch}
