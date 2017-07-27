#!/bin/bash
./xdyn tutorial_01_falling_ball.yml --dt 0.01 --tend 1 -o csv| python plot.py "tutorial_01" 0 3
./xdyn tutorial_02_fast_hydrostatic.yml --dt 0.1 --tend 10 -o csv| python plot.py "tutorial_02_fast_hydrostatic_z" 0 3
./xdyn tutorial_02_exact_hydrostatic.yml --dt 0.1 --tend 10 -o csv| python plot.py "tutorial_02_exact_hydrostatic_z" 0 3
./xdyn tutorial_02_fast_hydrostatic.yml --dt 0.1 --tend 10 -o csv| python plot.py "tutorial_02_fast_hydrostatic_y" 0 2
./xdyn tutorial_02_exact_hydrostatic.yml --dt 0.1 --tend 10 -o csv| python plot.py "tutorial_02_exact_hydrostatic_y" 0 2
./xdyn tutorial_03_waves.yml --dt 1 --tend 1 -w tutorial_03_results.yml
./xdyn tutorial_06_propulsion.yml controlled_forces_commands.yml --dt 0.1 --tend 20 -o csv| python plot.py "tutorial_06_propulsion_u" 0 4

mv *.svg "$1"
