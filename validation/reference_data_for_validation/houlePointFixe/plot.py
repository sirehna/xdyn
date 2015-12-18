# -*- coding: utf-8 -*-
#

import sys 
sys.path.append('../../macrosPython')

from fitCurves_Harmoniques import fitSignalHarmonic

tMin = 0.1

runName = ['run10.h5', 'run15.h5', 'run17.h5', 'run20.h5', 'run22.h5']
Hs = [1.1, 2.2, 3.3, 5.5, 9.9]
Tp = [6.3, 6.3, 6.3, 9.95, 9.95]

for iRun, run in enumerate(runName):
    fitSignalHarmonic([run], 'Anthineas', Tp[iRun], outputDir = 'graphes_fit/', tMinForFitting = 6*Tp[iRun], nbMode = 3, findPeriod = True, waveAmplitude = Hs[iRun]/2, tMin = tMin, title = '')
