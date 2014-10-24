#!/bin/bash
./sim tutorial_01_falling_ball.yml --dt 0.01 --tend 1 | python plot.py "tutorial_01" 0 3
./sim tutorial_02_fast_hydrostatic.yml --dt 0.1 --tend 10 | python plot.py "tutorial_02_fast_hydrostatic_z" 0 3
./sim tutorial_02_exact_hydrostatic.yml --dt 0.1 --tend 10 | python plot.py "tutorial_02_exact_hydrostatic_z" 0 3
./sim tutorial_02_fast_hydrostatic.yml --dt 0.1 --tend 10 | python plot.py "tutorial_02_fast_hydrostatic_y" 0 2
./sim tutorial_02_exact_hydrostatic.yml --dt 0.1 --tend 10 | python plot.py "tutorial_02_exact_hydrostatic_y" 0 2
./sim tutorial_03_waves.yml --dt 1 --tend 1 -w tutorial_03_results.yml
./sim tutorial_06_propulsion.yml --dt 0.1 --tend 20 -c controlled_forces_commands.yml | python plot.py "tutorial_06_propulsion_u" 0 4

mv *.svg "$1"
