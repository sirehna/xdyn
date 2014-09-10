#/bin/sh
# Script to generate profiling outputs
# It takes one argument: the name of program used to generate the gmon.out file
gprof -p -b $1 gmon.out > analysis_flat.txt
gprof -q -b $1 gmon.out > analysis_callgraph.txt
