#!/bin/python

def launch(executable, arguments):
    import os
    prefix = ''     if os.name=='nt' else './'
    suffix = '.exe' if os.name=='nt' else ''
    return os.system(prefix + executable + suffix + ' ' + arguments)

print(launch('xdyn', 'tutorial_03_waves.yml --dt 1 --tend 1 -w tutorial_03_results.h5'))
