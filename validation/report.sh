#!/bin/sh
docker run --rm -it -w /work -v $(pwd):/work xdyn python3 /usr/local/lib/python3.4/dist-packages/spt/report/JinJaReport.py $*
