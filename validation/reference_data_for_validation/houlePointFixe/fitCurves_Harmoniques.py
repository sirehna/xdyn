#! /usr/bin/python
# -*- coding: utf-8 -*-

# Import
import os
from scipy import *
from scipy.optimize import leastsq

# Import Graphique
import numpy as np
import matplotlib
matplotlib.use('Qt4Agg')
import matplotlib.pyplot as plt

# Import other files
from generateGraph import getDataForDisplacementAndVelocities
from generateGraph import getDataForWaves

from tools import isString

from toolsGraph import saveFigureSP6
from toolsGraph import generateCompFilename

def residuals(p, y, x, estimatedExcitationPeriod, findPeriod = True, nbMode = 1):
    if not findPeriod:
        p[0] = estimatedExcitationPeriod
    err = y-feval(x, p, nbMode = nbMode)
    return err

def feval(x, p, nbMode = 1):
    fourier = p[1]
    for i in range(nbMode):
        fourier = fourier + p[2*i+2]*cos(x*(i+1)*2*math.pi/p[0] ) + p[2*i+3]*sin(x*(i+1)*2*math.pi/p[0])
    return fourier

def fitAndGetInfos(x, y, estimatedExcitationPeriod, findPeriod = True, nbMode = 1, verbose = False):

    # Name
    pname=(['T', 'K0'])

    # Initial
    p0Liste=[estimatedExcitationPeriod, 0.0]

    # Add modes
    for i in range(nbMode):
        # cos
        name='C' + str(i+1)
        pname.append(name)   # name
        p0Liste.append(1)         # initial
        # sin
        name='S' + str(i+1)
        pname.append(name)   # name
        p0Liste.append(1)         # initial

    # Convert
    p0=array(p0Liste)

    # Least square
    plsq = leastsq(residuals, p0, args=(y, x, estimatedExcitationPeriod, findPeriod, nbMode), maxfev=2000)
    pEnd = plsq[0]

    if verbose:
        print('Parametres finaux')
        for i in range(len(pname)):
            print('\t{:s} = {:.4f} '.format(pname[i], pEnd[i]))
        print('')

    return [pEnd, x , feval(x, pEnd, nbMode = nbMode), pname]

def fitWavesHarmonic (inputData, estimatedExcitationPeriod, \
                      outputDir = '', \
                      xIndexWavePoint = 0, yIndexWavePoint = 0, \
                      tMinForFitting = 0, nbMode = 1, \
                      tMin = None, tMax = None, \
                      title = None, fontsize = 14, \
                      verbose = False):

    # Read wave data
    r = getDataForWaves(inputData[0], tMin = tMin, tMax = tMax)

    # Prepare texts
    titleFig = {'a':'Hauteur de houle'}
    filenameFig = {'a':'_Hauteur_houle'}
    filenameFigSmall = {'a':'_Hauteur'}

    xLabelFig = {'a':[r'$t\;[s]$']}

    yLabelFig = {}
    yLabelFig['a'] = [r'$z\;en\;({:.1f},{:.1f})\;[m]$'.format(r['x'][xIndexWavePoint], r['y'][yIndexWavePoint])]

    dataName = [d if isString(d) else d[0] if isinstance(d, (list, set, tuple)) else d.filename for d in inputData]

    if verbose:
        print('---------------------------------------')
        print('{:s} au point ({:.1f},{:.1f})'.format(titleFig['a'], r['x'][xIndexWavePoint], r['y'][yIndexWavePoint]))
        print('---------------------------------------')

    # Get time
    wave_time_t = r['timeSignal']

    # Get data
    ploti_t = r['z'][xIndexWavePoint][yIndexWavePoint]

    # Compute xindfit
    xindfit = 0.
    for k,t in enumerate(wave_time_t):
        if t >= tMinForFitting:
            xindfit = k
            break

    # Fit
    [infos, time_fit, ploti_fit, pname] = fitAndGetInfos(wave_time_t[xindfit:], ploti_t[xindfit:], estimatedExcitationPeriod, nbMode = nbMode, verbose = verbose)

    # Plot
    index = xLabelFig.keys()[0]
    fig, ax1 = plt.subplots()
    plt.grid(True)
    ax1.plot(wave_time_t, ploti_t, linewidth=2)
    ax1.plot(time_fit, ploti_fit, '--', linewidth=2)
    ax1.set_xlabel(xLabelFig[index][0], fontsize = fontsize)
    ax1.set_ylabel(yLabelFig[index][0], fontsize = fontsize)
    ax1.legend(['Initial', 'Interp'], loc='upper right', fontsize = fontsize)
    if title is not None:
        if len(title) == 0:
            outputTitle = u'Elévation de surface libre'
        else:
            outputTitle = title
        plt.text(0.5, 1.05, outputTitle, horizontalalignment = 'center', fontsize = fontsize, transform = ax1.transAxes)
    outputFilename = generateCompFilename(dataName, \
                                          trailingPattern = filenameFig[index] + '_fit', \
                                          trailingPatternSmall = filenameFigSmall[index],
                                          header = 'Comp_') + '.png'
    if len(outputDir)>0:
        if not os.path.exists(outputDir):
            os.makedirs(outputDir)
        outputFilename = outputDir + outputFilename
    plt.savefig(outputFilename)
    plt.close()

    # Post-process
    signalAmplitude = np.sqrt(np.power(infos[2],2) + np.power(infos[3],2))
    signalPeriod = infos[0]
    signalPhaseDeg = np.degrees(np.arctan2(infos[2],infos[3]))
    signalMax = max(ploti_t[xindfit:])
    signalMin = min(ploti_t[xindfit:])

    # Write post-processing
    filePost = open(os.path.splitext(outputFilename)[0] + '.dat' , 'w')

    filePost.write('---Resultats---\n')
    outputUnit = yLabelFig[index][0][yLabelFig[index][0].find('['):yLabelFig[index][0].find(']')+1]
    filePost.write('Amplitude signal {:s} : \t{:.2f}\n'.format(outputUnit, signalAmplitude))
    filePost.write('Periode signal [s] : \t{:.2f}\n'.format(signalPeriod))
    filePost.write('Phase signal [deg] : \t{:.2f}\n'.format(signalPhaseDeg))
    filePost.write('Max signal {:s} : \t\t{:.2f}\n'.format(outputUnit, signalMax))
    filePost.write('Min signal {:s} : \t\t{:.2f}\n'.format(outputUnit, signalMin))

    filePost.write('\n')
    filePost.write('---Coefficients harmoniques---\n')
    for i in range(len(pname)):
        filePost.write('{:s} : \t{:.2f}\n'.format(pname[i], infos[i]))

    filePost.close()

    # Return wave fit
    res = {}
    res['waveAmplitude'] = signalAmplitude
    res['wavePeriod'] = signalPeriod
    res['wavePhaseDeg'] = signalPhaseDeg
    return res

def fitSignalHarmonic(inputData, boatName, estimatedExcitationPeriod, \
                      outputDir = '', \
                      fitVelocities = False, fitGlobalVelocities = False, \
                      tMinForFitting = 0, nbMode = 1, findPeriod = True, \
                      tMin = None, tMax = None, \
                      yMin = None, yMax = None, \
                      title = None, simplePlotFontsize = 14, subplotFontsize = 18,\
                      waveFile = None, xIndexWavePoint = 0, yIndexWavePoint = 0, \
                      waveAmplitude = 1., wavePhaseDeg = 0., \
                      sep = ' ', verbose = False):

    # Get wave fit
    if waveFile is not None:
        rWave = fitWavesHarmonic(waveFile, estimatedExcitationPeriod, outputDir = outputDir,\
                             xIndexWavePoint = xIndexWavePoint, yIndexWavePoint = yIndexWavePoint,\
                             tMinForFitting = tMinForFitting, nbMode = nbMode, tMin = tMin, tMax = tMax,\
                             title = title, fontsize = simplePlotFontsize, verbose = verbose)
        waveAmplitude = rWave['waveAmplitude']
        wavePeriod = rWave['wavePeriod']
        wavePhaseDeg = rWave['wavePhaseDeg']
        estimatedExcitationPeriod = wavePeriod
    else:
        wavePeriod = estimatedExcitationPeriod

    # Prepare texts
    nInputData = len(inputData)
    if nInputData != 1:
        return

    dataName = [d if isString(d) else d[0] if isinstance(d, (list, set, tuple)) else d.filename for d in inputData]

    tMin = [tMin] * nInputData if not isinstance(tMin, (list, set, tuple)) else tMin
    tMax = [tMax] * nInputData if not isinstance(tMax, (list, set, tuple)) else tMax

    titleHead = ''
    if title is not None:
        if len(title) == 0:
            titleHead = os.path.splitext(dataName[0])[0]
        else:
            titleHead = title
    titleFig = {'a':titleHead + '\n\nDéplacements', 'b':titleHead + '\n\nVitesses locales', 'c':titleHead + '\n\nVitesses NED'}
    filenameFig = {'a':'_Deplacements', 'b':'_Vitesses_locales', 'c':'_Vitesses_NED'}
    filenameFigSmall = {'a':'_Dep', 'b':'_VitLoc', 'c':'_VitNed'}

    xLabelFig = {'a':[r'$t\;[s]$'] * 6, 'b': [r'$t\;[s]$'] * 6, 'c':[r'$t\;[s]$'] * 6}

    yLabelFig = {}
    yLabelFig['a'] = [r'$X\;[m]$', r'$Y\;[m]$', r'$Z\;[m]$', r'$\phi\;[deg]$', r'$\theta\;[deg]$', r'$\psi\;[deg]$']
    yLabelFig['b'] = [r'$U\;[m/s]$', r'$V\;[m/s]$', r'$W\;[m/s]$', r'$P\;[deg/s]$', r'$Q\;[deg/s]$', r'$R\;[deg/s]$']
    yLabelFig['c'] = [r'$\dot X\;[m/s]$', r'$\dot Y\;[m/s]$', r'$\dot Z\;[m/s]$', r'$\dot \phi\;[deg/s]$', r'$\dot \theta\;[deg/s]$', r'$\dot \psi\;[deg/s]$']

    yFigName = {}
    yFigName['a'] = ['Cavalement', 'Embardee', 'Pilonnement', 'Roulis', 'Tangage', 'Lacet']
    yFigName['b'] = ['U', 'V', 'W', 'P', 'Q', 'R']
    yFigName['c'] = ['Xp', 'Yp', 'Zp', 'Phip', 'Thetap', 'Psip']

    # Read data
    r = [getDataForDisplacementAndVelocities(data, boatName, tMin = tMin[i], tMax = tMax[i]) for i, data in enumerate(inputData)][0]

    # Get time
    time_t = r['timeSignal']

    # Get data
    dataFig = {}
    dataFig['a'] = [r['X'], r['Y'], r['Z'], r['Phi'] * 180.0 / np.pi, r['Theta'] * 180.0 / np.pi, r['Psi'] * 180.0 / np.pi]
    if fitVelocities:
        dataFig['b'] = [r['U'], r['V'], r['W'], r['P'] * 180.0 / np.pi, r['Q'] * 180.0 / np.pi, r['R'] * 180.0 / np.pi]
    if fitGlobalVelocities:
        dataFig['c'] = [r['Xp'], r['Yp'], r['Zp'], r['Phip'] * 180.0 / np.pi, r['Thetap'] * 180.0 / np.pi, r['Psip'] * 180.0 / np.pi]
    nDataFig = len(dataFig)

    legendFig = {'a':[], 'b':[], 'c':[]}
    legendFig2ndAxis = {'a':[], 'b':[], 'c':[]}

    # Check yMin and yMax
    if yMin is not None:
        if len(yMin) != nDataFig * len(dataFig[dataFig.keys()[0]]):
            print('yMin is not defined correctly (wrong number of values): it will be ignored')
            yMin = None
    if yMax is not None:
        if len(yMax) != nDataFig * len(dataFig[dataFig.keys()[0]]):
            print('yMax is not defined correctly (wrong number of values): it will be ignored')
            yMax = None

    # Loop on data
    res = []
    for j in range(nDataFig):
        fig = plt.figure(j, figsize = (24, 14))
        index = dataFig.keys()[j]

        signalAmplitude = ''
        signalAmplitudeAdim = []
        signalPeriod = ''
        signalPhaseDeg = ''
        signalPhaseDegAdim = []
        signalMax = ''
        signalMin = ''
        signalNameAndUnit = ''
        signalHarmonicCoefficients = ['' for i in range(1+2*nbMode)]

        # Loop on motions
        nMotions = len(dataFig[index])

        for iMotion in range(nMotions):
            if verbose:
                print('---------------------------------------')
                print(yFigName[index][iMotion])
                print('---------------------------------------')

            ploti_t = dataFig[index][iMotion]

            # Compute xindfit
            xindfit = 0.
            for k,t in enumerate(time_t):
                if t >= tMinForFitting:
                    xindfit = k
                    break

            # Estimate period
            if findPeriod:
                signalFFT = np.fft.fft(ploti_t[xindfit:]-np.mean(ploti_t[xindfit:]))
                freqs=np.fft.fftfreq(len(signalFFT), time_t[-1]-time_t[-2])
                estimatedExcitationPeriod = abs(1/freqs[argmax(abs(signalFFT))])

            # Fit
            [infos, time_fit, ploti_fit, pname] = fitAndGetInfos(time_t[xindfit:], ploti_t[xindfit:], estimatedExcitationPeriod, nbMode = nbMode, findPeriod = findPeriod, verbose = verbose)

            # Create waves
            waves_t = waveAmplitude * np.sin(2 * np.pi / wavePeriod * time_t + np.pi / 180. * wavePhaseDeg)

            # Plot
            ax1 = plt.subplot(230 + (iMotion + 1))
            plt.grid(True)
            hp1, = ax1.plot(time_t, ploti_t, linewidth=2)
            hp2, = ax1.plot(time_fit, ploti_fit, '--', linewidth=2)
            ax1.set_xlabel(xLabelFig[index][iMotion], fontsize = subplotFontsize)
            ax1.set_ylabel(yLabelFig[index][iMotion], fontsize = subplotFontsize)
            ax2 = ax1.twinx()
            hp3, = ax2.plot(time_t, waves_t, 'r')
            ax2.set_ylabel(r'$Hauteur\;de\;houle\;[m]$', fontsize = subplotFontsize)
            if yMin is not None and yMin[j*nMotions+iMotion]!='':
                ax1.set_ylim(bottom=yMin[j*nMotions+iMotion])
            if yMax is not None and yMax[j*nMotions+iMotion]!='':
                ax1.set_ylim(top=yMax[j*nMotions+iMotion])
            ax2.set_ylim([-waveAmplitude*2,waveAmplitude*2])
            if iMotion == 0:
                legendFig[index].append(hp1)
                legendFig[index].append(hp2)
                legendFig2ndAxis[index].append(hp3)

            # Post-process
            signalAmplitude = signalAmplitude + '{:s}{:.4f}'.format('' if iMotion == 0 else sep, np.sqrt(np.power(infos[2],2) + np.power(infos[3],2)))
            signalAmplitudeAdim.append('{:.4E}'.format(np.sqrt(np.power(infos[2],2) + np.power(infos[3],2))/waveAmplitude))
            signalPeriod = signalPeriod + '{:s}{:.4f}'.format('' if iMotion == 0 else sep, infos[0])
            signalPhaseDegTemp = np.degrees(np.arctan2(infos[2],infos[3])) - wavePhaseDeg
            while (signalPhaseDegTemp < 0.):
                signalPhaseDegTemp = signalPhaseDegTemp + 360.
            while (signalPhaseDegTemp >= 360.):
                signalPhaseDegTemp = signalPhaseDegTemp - 360.
            signalPhaseDeg = signalPhaseDeg + '{:s}{:.4f}'.format('' if iMotion == 0 else sep, signalPhaseDegTemp)
            signalPhaseDegAdim.append('{:.4E}'.format(signalPhaseDegTemp))
            signalMax = signalMax + '{:s}{:.4f}'.format('' if iMotion == 0 else sep, max(ploti_t[xindfit:]))
            signalMin = signalMin + '{:s}{:.4f}'.format('' if iMotion == 0 else sep, min(ploti_t[xindfit:]))
            signalNameAndUnit = signalNameAndUnit + '{:s}{:s}{:s}'\
                    .format('' if iMotion == 0 else sep, yFigName[index][iMotion], yLabelFig[index][iMotion][yLabelFig[index][iMotion].find('['):yLabelFig[index][iMotion].find(']')+1])
            for iCoef in range(len(pname)-1):
                signalHarmonicCoefficients[iCoef] = signalHarmonicCoefficients[iCoef] + '{:s}{:.4f}'.format('' if iMotion == 0 else sep, infos[iCoef+1])
            if iMotion == nMotions-1:
                res = [signalAmplitudeAdim, signalPhaseDegAdim]

        # Finalise, save and close plot
        plt.subplots_adjust(wspace = 0.5)
        plt.figtext(0.5, 0.93, unicode(titleFig[index], 'utf-8'), horizontalalignment = 'center', fontsize = subplotFontsize)
        legend = ['Initial', 'Interp']
        nLegend = len(legend)
        l = plt.figlegend(legendFig[index], legend, bbox_to_anchor=(0.55,0.), loc = 4, ncol = (nLegend + 1) if nLegend < 6 else (nLegend / 2))
        for t in l.texts:
            t.set_fontsize(subplotFontsize)
        legend2ndAxis = ['Houle']
        nLegend = len(legend2ndAxis)
        l = plt.figlegend(legendFig2ndAxis[index], legend2ndAxis, bbox_to_anchor=(0.55,0.), loc = 3, ncol = (nLegend + 1) if nLegend < 6 else (nLegend / 2))
        for t in l.texts:
            t.set_fontsize(subplotFontsize)
        outputFilename = generateCompFilename(dataName, \
                                              trailingPattern = filenameFig[index] + '_fit', \
                                              trailingPatternSmall = filenameFigSmall[index],
                                              header = 'Comp_')
        if len(outputDir)>0:
            if not os.path.exists(outputDir):
                os.makedirs(outputDir)
            outputFilename = outputDir + outputFilename
        if verbose:
            print("Output image : {0}".format(outputFilename))
        saveFigureSP6(fig, outputFilename, xLabelFig, yLabelFig)
        plt.close(fig)

        # Write post-processing for direct signal
        filePost = open(outputFilename + '.dat' , 'w')

        filePost.write('---Parametres d\'entree---\n')
        filePost.write('T houle [s] : \t\t\t{:.2f}\n'.format(wavePeriod))
        filePost.write('Amplitude houle [m] : \t{:.2f}\n'.format(waveAmplitude))
        filePost.write('\n')

        filePost.write('---Resultats---\n')
        filePost.write('Etats\n{:s}\n'.format(signalNameAndUnit))
        filePost.write('\nAmplitude signal\n{:s}\n'.format(signalAmplitude))
        filePost.write('\nPeriode signal [s] {:s}\n{:s}\n'.format('(imposee) ' if findPeriod is False else '', signalPeriod))
        filePost.write('\nPhase signal [deg]\n{:s}\n'.format(signalPhaseDeg))
        filePost.write('\nMax signal\n{:s}\n'.format(signalMax))
        filePost.write('\nMin signal\n{:s}\n'.format(signalMin))

        filePost.write('\n')
        filePost.write('---Coefficients harmoniques---')
        for iCoef in range(len(pname)-1):
            filePost.write('\n{:s}\n{:s}\n'.format(pname[iCoef+1], signalHarmonicCoefficients[iCoef]))

        filePost.close()

        return res
