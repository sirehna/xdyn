#!/bin/bash
./sim tutorial_01_falling_ball.yml --dt 0.01 --tend 1 | python plot.py "tutorial_01" 0 3
./sim tutorial_02_fast_hydrostatic.yml --dt 0.1 --tend 10 | python plot.py "tutorial_02_fast_hydrostatic_z" 0 3
./sim tutorial_02_exact_hydrostatic.yml --dt 0.1 --tend 10 | python plot.py "tutorial_02_exact_hydrostatic_z" 0 3
./sim tutorial_02_fast_hydrostatic.yml --dt 0.1 --tend 10 | python plot.py "tutorial_02_fast_hydrostatic_y" 0 2
./sim tutorial_02_exact_hydrostatic.yml --dt 0.1 --tend 10 | python plot.py "tutorial_02_exact_hydrostatic_y" 0 2
./sim tutorial_03_waves.yml --dt 1 --tend 1 -w tutorial_04_results.yml

mv *.svg "$1"
