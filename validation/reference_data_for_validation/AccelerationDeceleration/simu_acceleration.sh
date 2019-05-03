#!/bin/sh
xdyn test_ship_acceleration.yml controlled_forces_commands.yml --dt 0.2 --tend 60 -o acceleration.h5
