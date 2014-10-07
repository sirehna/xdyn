#!/bin/bash
./sim tutorial_01_falling_ball.yml --dt 0.01 --tend 1 | python ../../../postprocessing/plot.py "tutorial_01" 0 3
./sim tutorial_02_hydrostatic.yml --dt 0.01 --tend 1 | python ../../../postprocessing/plot.py "tutorial_02" 0 3
./sim tutorial_03_exact_hydrostatic.yml --dt 0.01 --tend 1 | python ../../../postprocessing/plot.py "tutorial_03" 0 3
./sim tutorial_04_waves.yml --dt 1 --tend 1 -w tutorial_04_results.yml
./sim tutorial_05_froude_krylov.yml --dt 0.1 --tend 1 | python ../../../postprocessing/plot.py "tutorial_05" 0 3

mv *.svg $1
