#!/bin/sh

# Get SSC
docker build -f Dockerfile.ssc -t get_ssc .
docker run -it --rm -v $(pwd):/out --net=host get_ssc

# Build X-DYN container
docker build -f Dockerfile -t xdyn .

# Run container to build X-DYN
docker run --rm -v $(pwd):/opt/share -it xdyn ninja

# Launch tests
docker run --rm -it -v $(pwd):/opt/share xdyn ./run_all_tests --gtest_filter='GZCurveTest.should_throw_if_ship_is_denser_than_water'
