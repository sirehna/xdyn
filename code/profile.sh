#/bin/sh
gprof -p -b $1 gmon.out > analysis_flat.txt
gprof -q -b $1 gmon.out > analysis_callgraph.txt
