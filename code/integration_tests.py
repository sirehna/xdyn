#!/bin/python
import os

prefix = ''     if os.name=='nt' else './'
suffix = '.exe' if os.name=='nt' else ''

def launch(executable, arguments):
    return os.system(prefix + executable + suffix + ' ' + arguments)

print(launch('sim', 'tutorial_03_waves.yml --dt 1 --tend 1 -w tutorial_03_results.h5'))
