#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
import argparse
import matplotlib
import math
import numpy as np
from numpy import exp, log


def which(program):
    """ retur full path to program if found, else return None
    """
    def is_exe(fpath):
        return os.path.isfile(fpath) and os.access(fpath, os.X_OK)
    fpath, _ = os.path.split(program)
    if fpath:
        if is_exe(program):
            return program
    else:
        for path in os.environ["PATH"].split(os.pathsep):
            exe_file = os.path.join(path, program)
            if is_exe(exe_file):
                return exe_file
    return None


matplotlib.use('AGG')
matplotlib.rc('grid', linewidth=0.5, linestyle='--', color='#bdbdbd')
matplotlib.rc('xtick', labelsize=14)
matplotlib.rc('ytick', labelsize=14)
latex_program = 'latex' + '.exe' if sys.platform == 'win32' else ''
matplotlib.rc('text', usetex = True if which(latex_program) else False)
matplotlib.rc('font', **{'family': 'serif', 'serif': ['Palatino'], 'size': 14})

# Must be after matplotlib.use
from pylab import *


def plotOptionsL():
    typeList = ['bx-', 'gp-', 'rs-', 'co-', 'mD-', 'yH-', 'k*-',
                'gp--', 'rs--', 'co--', 'mD--', 'yH--', 'k*--']
    return typeList


def plotOptions(n=0, useColor=False):
    d = [{'c': 'b', 'linestyle': '-', 'marker': 'x', 'markevery': 11},
         {'c': 'g', 'linestyle': '-', 'marker': 'p', 'markevery': 13},
         {'c': 'r', 'linestyle': '-', 'marker': 's', 'markevery': 17},
         {'c': 'm', 'linestyle': '-', 'marker': 'o', 'markevery': 15},
         {'c': 'y', 'linestyle': '-', 'marker': 'D', 'markevery': 19},
         {'c': 'k', 'linestyle': '-', 'marker': '^', 'markevery': 21},
         {'c': 'k', 'linestyle': '-', 'marker': '<', 'markevery': 23}]
    opt = d[n % len(d)]
    if not useColor:
        opt['c'] = 'k'
    return opt


def getDefaultExtensions():
    return ['svg', 'eps', 'png']


def generateFigureFileNames(imageName, extensions = getDefaultExtensions()):
    if isinstance(extensions, (str, )):
        extensions = [extensions]
    return [imageName + e if e.startswith('.') else imageName + '.' + e for e in extensions]


PI = np.pi
def POW2(X):
    return X * X


def POW3(X):
    return X * X * X


def POW4(X):
    return X * X * X * X


def POW5(X):
    return X * X * X * X * X


def POW6(X):
    return X * X * X * X * X * X


def psdPiersonMoskowitz(W, V):
    g = 9.81
    A = 0.0081 * g * g
    B = 0.74 * POW4(g / V)
    psd = A / POW5(W) * exp(-B / POW4(W))
    return psd


def psdJonswap(W, Hs, Tp, gamma = 3.3):
    W0 = 2.0 * PI / Tp
    sigma_a = 0.07
    sigma_b = 0.09
    W = np.array(W)
    sigma = np.zeros(W.size)
    sigma[W <= W0] = sigma_a
    sigma[W > W0] = sigma_b
    alpha = POW4(W0 / W)
    Awm5 = 5.0 / 16.0 * alpha / W * Hs * Hs
    Bwm4 = 1.25 * alpha
    coeff = (1 - 0.287 * log(gamma))
    r = exp(-0.5 * POW2((W - W0) / (sigma * W0)))
    psd = coeff * Awm5 * exp(-Bwm4) * pow(gamma, r)
    return psd


def psdOchi(W, Hs, Tp, ochiLambda):
    W0 = 2.0 * PI / Tp
    alpha = POW4(W0 / W)
    l414 = ochiLambda + 0.25
    psd = 0.25 * pow(l414 * alpha, ochiLambda) / (W * math.gamma(ochiLambda)) * (Hs * Hs) * exp(-l414 * alpha)
    return psd


def psdBretschneider(W, Hs, Tp):
    W0 = 2.0 * PI / Tp
    alpha = POW4(W0 / W)
    psd = 0.3125 * (alpha / W) * (Hs * Hs) * exp(-1.25 * alpha)
    return psd


def psdITTC(W, Hs):
    g = 9.81
    A = 0.0081 * g * g
    B = 3.11 / (Hs * Hs)
    psd = A / POW5(W) * exp(-B / POW4(W))
    return psd

def psdVignatBovis(W, Hs, Tp):
    TpW5 = POW5(Tp * W)
    psd = 4001.26 * POW2(Hs) / (TpW5 * W) * exp(-11751.15 / TpW5)
    return psd

def psdOneraTILV10(W, Hs, Tp):
    TpW5 = POW5(Tp * W)
    psd = 3672.236 * POW2(Hs) / (TpW5 * W) * exp(-11751.15 / TpW5)
    return psd

def psdGaussian(W, Hs, Tp, sigma):
    W0 = 2.0 * PI / Tp
    A = 0.0625 * Hs * Hs / (sigma * pow(2 * PI, 1.5))
    psd = A * exp(-POW2(W - W0) / (8.0 * POW2(PI * sigma)))
    return psd

def imagePiersonMoskowitz(extensions=getDefaultExtensions(), useColor = False):
    dw = 0.01
    W = np.arange(dw, 3 + dw, dw)
    psd07 = psdPiersonMoskowitz(W, 7)
    psd10 = psdPiersonMoskowitz(W, 10)
    psd15 = psdPiersonMoskowitz(W, 15)
    fig = figure()
    grid()
    plot(W, psd07, label=r'$V= 7m/s$', **plotOptions(0, useColor))
    plot(W, psd10, label=r'$V=10m/s$', **plotOptions(1, useColor))
    plot(W, psd15, label=r'$V=15m/s$', **plotOptions(2, useColor))
    legend(loc = 'right')
    xlabel(r'$\omega$ (rad/s)')
    ylabel(r'Power Spectral Density ($m^2.s$)')
    title(r'Pierson-Moskowitz spectrum')
    [savefig(f) for f in generateFigureFileNames('spectrumPiersonMoskowitz', extensions)]
    matplotlib.pyplot.close(fig)


def imageJonswap(Hs = 1, Tp = 10, extensions=getDefaultExtensions(), useColor = False):
    dw = 0.01
    W = np.arange(dw, 3 + dw, dw)
    psd2 = psdJonswap(W, Hs, Tp, 2)
    psd3 = psdJonswap(W, Hs, Tp, 3)
    psd7 = psdJonswap(W, Hs, Tp, 7)
    fig = figure()
    grid()
    plot(W, psd2, label=r'$\gamma=2$', **plotOptions(0, useColor))
    plot(W, psd3, label=r'$\gamma=3$', **plotOptions(1, useColor))
    plot(W, psd7, label=r'$\gamma=7$', **plotOptions(2, useColor))
    legend(loc = 'right')
    xlabel(r'$\omega$ (rad/s)')
    ylabel(r'Power Spectral Density ($m^2.s$)')
    title(r'JONSWAP spectrum - $H_s=' + str(Hs) + 'm$ - $T_p=' + str(Tp) + 's$')
    [savefig(f) for f in generateFigureFileNames('spectrumJonswap', extensions)]
    matplotlib.pyplot.close(fig)


def imageOchi(Hs = 1, Tp = 10, extensions=getDefaultExtensions(), useColor = False):
    dw = 0.01
    W = np.arange(dw, 3 + dw, dw)
    psd05 = psdOchi(W, Hs, Tp, ochiLambda = 0.5)
    psd15 = psdOchi(W, Hs, Tp, ochiLambda = 1.5)
    psd30 = psdOchi(W, Hs, Tp, ochiLambda = 3.0)
    fig = figure()
    grid()
    plot(W, psd05, label=r'$\lambda=0.5$', **plotOptions(0, useColor))
    plot(W, psd15, label=r'$\lambda=1.5$', **plotOptions(1, useColor))
    plot(W, psd30, label=r'$\lambda=3.0$', **plotOptions(2, useColor))
    legend(loc = 'right')
    xlabel(r'$\omega$ (rad/s)')
    ylabel(r'Power Spectral Density ($m^2.s$)')
    title(r'Ochi spectrum - $H_s=' + str(Hs) + 'm$ - $T_p=' + str(Tp) + 's$')
    [savefig(f) for f in generateFigureFileNames('spectrumOchi', extensions)]
    matplotlib.pyplot.close(fig)


def imageBretschneider(Hs = 1, Tp = None, extensions=getDefaultExtensions(), useColor = False):
    dw = 0.01
    W = np.arange(dw, 3 + dw, dw)
    fig = figure()
    grid()
    if Tp is None:
        psd05 = psdBretschneider(W, Hs, Tp=5)
        psd10 = psdBretschneider(W, Hs, Tp=10)
        psd15 = psdBretschneider(W, Hs, Tp=15)
        plot(W, psd05, label=r'$T_p=5s$' , **plotOptions(0, useColor))
        plot(W, psd10, label=r'$T_p=10s$', **plotOptions(1, useColor))
        plot(W, psd15, label=r'$T_p=15s$', **plotOptions(2, useColor))
    elif isinstance(Tp, (float, int)):
        psd = psdBretschneider(W, Hs, Tp = Tp)
        plot(W, psd, label=r'$T_p=' + str(Tp) + 's$', **plotOptions(0, useColor))
    elif isinstance(Tp, (list, set, tuple)):
        for i, tp in enumerate(Tp):
            psd = psdBretschneider(W, Hs, Tp = tp)
            plot(W, psd, label=r'$T_p=' + str(tp) + 's$', **plotOptions(i, useColor))
    legend(loc = 'right')
    xlabel(r'$\omega$ (rad/s)')
    ylabel(r'Power Spectral Density ($m^2.s$)')
    title(r'ISSC Bretschneider spectrum - $H_s=' + str(Hs) + 'm$')
    [savefig(f) for f in generateFigureFileNames('spectrumBretschneider', extensions)]
    matplotlib.pyplot.close(fig)


def imageITTC(Hs = None, extensions=getDefaultExtensions(), useColor = False):
    dw = 0.01
    W = np.arange(dw, 3 + dw, dw)
    fig = figure()
    grid()
    if Hs is None:
        psd05 = psdITTC(W, Hs = 0.5)
        psd10 = psdITTC(W, Hs = 1.0)
        psd15 = psdITTC(W, Hs = 1.5)
        plot(W, psd05, label=r'$H_s=0.5m$', **plotOptions(0, useColor))
        plot(W, psd10, label=r'$H_s=1.0m$', **plotOptions(1, useColor))
        plot(W, psd15, label=r'$H_s=1.5m$', **plotOptions(2, useColor))
    elif isinstance(Hs, (float, int)):
        psd = psdITTC(W, Hs = Hs)
        plot(W, psd, label=r'$H_s=' + str(Hs) + 'm$', **plotOptions(0, useColor))
    elif isinstance(Hs, (list, set, tuple)):
        for i, hs in enumerate(Hs):
            psd = psdITTC(W, Hs = hs)
            plot(W, psd, label=r'$H_s=' + str(hs) + 'm$', **plotOptions(i, useColor))
    legend(loc = 'right')
    xlabel(r'$\omega$ (rad/s)')
    ylabel(r'Power Spectral Density ($m^2.s$)')
    title(r'ITTC spectrum')
    [savefig(f) for f in generateFigureFileNames('spectrumITTC', extensions)]
    matplotlib.pyplot.close(fig)


def imageVignatBovis(Hs = None, Tp = 10, extensions=getDefaultExtensions(), useColor = False):
    dw = 0.01
    W = np.arange(dw, 3 + dw, dw)
    fig = figure()
    grid()
    if Hs is None:
        psd05 = psdVignatBovis(W, Hs = 0.5, Tp = Tp)
        psd10 = psdVignatBovis(W, Hs = 1.0, Tp = Tp)
        psd15 = psdVignatBovis(W, Hs = 1.5, Tp = Tp)
        plot(W, psd05, label=r'$H_s=0.5m$', **plotOptions(0, useColor))
        plot(W, psd10, label=r'$H_s=1.0m$', **plotOptions(1, useColor))
        plot(W, psd15, label=r'$H_s=1.5m$', **plotOptions(2, useColor))
    elif isinstance(Hs, (float, int)):
        psd = psdVignatBovis(W, Hs = Hs, Tp = Tp)
        plot(W, psd, label=r'$H_s=' + str(Hs) + 'm$', **plotOptions(0, useColor))
    elif isinstance(Hs, (list, set, tuple)):
        for i, hs in enumerate(Hs):
            psd = psdVignatBovis(W, Hs = hs, Tp = Tp)
            plot(W, psd, label=r'$H_s=' + str(hs) + 'm$', **plotOptions(i, useColor))
    legend(loc = 'right')
    xlabel(r'$\omega$ (rad/s)')
    ylabel(r'Power Spectral Density ($m^2.s$)')
    title(r'Vignat Bovis spectrum')
    [savefig(f) for f in generateFigureFileNames('spectrumVignatBovis', extensions)]
    matplotlib.pyplot.close(fig)


def imageOneraTILV10(Hs=None, Tp=10, extensions=getDefaultExtensions(), useColor = False):
    dw = 0.01
    W = np.arange(dw, 3 + dw, dw)
    fig = figure()
    grid()
    if Hs is None:
        psd05 = psdOneraTILV10(W, Hs=0.5, Tp=Tp)
        psd10 = psdOneraTILV10(W, Hs=1.0, Tp=Tp)
        psd15 = psdOneraTILV10(W, Hs=1.5, Tp=Tp)
        plot(W, psd05, label=r'$H_s=0.5m - T_p=' + str(Tp) + 's$', **plotOptions(0, useColor))
        plot(W, psd10, label=r'$H_s=1.0m - T_p=' + str(Tp) + 's$', **plotOptions(1, useColor))
        plot(W, psd15, label=r'$H_s=1.5m - T_p=' + str(Tp) + 's$', **plotOptions(2, useColor))
    elif isinstance(Hs, (float, int)):
        psd = psdOneraTILV10(W, Hs = Hs, Tp = Tp)
        plot(W, psd, label=r'$H_s=' + str(Hs) + 'm - T_p=' + str(Tp) + 's$', **plotOptions(0, useColor))
    elif isinstance(Hs, (list, set, tuple)):
        for i, hs in enumerate(Hs):
            psd = psdOneraTILV10(W, Hs = hs, Tp = Tp)
            plot(W, psd, label=r'$H_s=' + str(hs) + 'm - T_p=' + str(Tp) + 's$', **plotOptions(i, useColor))
    legend(loc = 'right')
    xlabel(r'$\omega$ (rad/s)')
    ylabel(r'Power Spectral Density ($m^2.s$)')
    title(r'Onera TILV10 spectrum')
    [savefig(f) for f in generateFigureFileNames('spectrumOneraTILV10', extensions)]
    matplotlib.pyplot.close(fig)


def imageGaussian(Hs=1, Tp=10, sigma=None, extensions=getDefaultExtensions(), useColor = False):
    dw = 0.01
    W = np.arange(dw, 3 + dw, dw)
    fig = figure()
    grid()
    if sigma is None:
        psd001 = psdGaussian(W, Hs, Tp, sigma = 0.01)
        psd002 = psdGaussian(W, Hs, Tp, sigma = 0.02)
        psd004 = psdGaussian(W, Hs, Tp, sigma = 0.04)
        plot(W, psd001, label=r'$\sigma=0.01$', **plotOptions(0, useColor))
        plot(W, psd002, label=r'$\sigma=0.02$', **plotOptions(1, useColor))
        plot(W, psd004, label=r'$\sigma=0.04$', **plotOptions(2, useColor))
    elif isinstance(sigma, (float, int)):
        psd = psdGaussian(W, Hs, Tp, sigma = sigma)
        plot(W, psd, label=r'$\sigma=' + str(sigma) + '$', **plotOptions(0, useColor))
    elif isinstance(sigma, (list, set, tuple)):
        for sig in sigma:
            psd = psdGaussian(W, Hs, Tp, sigma = sig)
            plot(W, psd, label=r'$\sigma=' + str(sig) + '$', **plotOptions(0, useColor))
    legend(loc = 'right')
    xlabel(r'$\omega$ (rad/s)')
    ylabel(r'Power Spectral Density ($m^2.s$)')
    title(r'Gaussian spectrum - $H_s=' + str(Hs) + 'm$ - $T_p=' + str(Tp) + 's$')
    [savefig(f) for f in generateFigureFileNames('spectrumGaussian', extensions)]
    matplotlib.pyplot.close(fig)


def imageMonochromatique(extensions=getDefaultExtensions(), useColor = False):
    fig = figure()
    grid()
    dt = 0.05
    A = 3.5
    t = np.arange(0.0,20+dt,dt)
    T = 10
    y = A*cos(-2*np.pi/T*t-0.3)
    plot(t,y,  label =r'', **{'c':'k', 'linestyle':'-'})
    ax = gca()
    ax.arrow(5, 0, 0, A-0.25, head_width=0.5, head_length=0.25, fc='k', ec='k')
    ax.text(4, A/2, "A")
    ax.arrow(9.5, A+0.15, T, 0, head_width=0.25, head_length=0.25, fc='k', ec='k')
    ax.arrow(9.5+T, A+0.15, -T, 0, head_width=0.25, head_length=0.25, fc='k', ec='k')
    ax.text(9.5+T/2, 0.9*A, "T")
    xlabel(r'$t$ (s)')
    ylabel(r'$\eta \left( t \right)$ ($m$)')
    title(u'')
    [savefig(f) for f in generateFigureFileNames('waveMonochromatique', extensions)]
    matplotlib.pyplot.close(fig)


def imageBichromatique(extensions=getDefaultExtensions(), useColor = False):
    fig = figure()
    grid()
    dt = 0.025
    A1,A2 = 3.5, 1.3
    t = np.arange(0.0,100+dt,dt)
    T1,T2 = 10,17
    y = A1*cos(-2*np.pi/T1*t-0.3) + A2*cos(-2*np.pi/T2*t-0.7)
    plot(t,y,  label =r'', **{'c':'k', 'linestyle':'-'})
    xlabel(r'$t$ (s)')
    ylabel(r'$\eta \left( t \right)$ ($m$)')
    title(u'')
    [savefig(f) for f in generateFigureFileNames('waveBichromatique', extensions)]
    matplotlib.pyplot.close(fig)


def imageComparison1(Hs=1, Tp=10, extensions=getDefaultExtensions(), useColor=False):
    dw = 0.01
    W = np.arange(dw, 5 + dw, dw)
    fig = figure()
    grid()
    plot(W, psdPiersonMoskowitz(W, 7), label=r'Pierson-Moskovitz, $V=7m/s$', **plotOptions(0, useColor))
    plot(W, psdITTC(W, Hs), label=r'ITTC, $H_s={0}m$'.format(Hs), **plotOptions(1, useColor))
    plot(W, psdBretschneider(W, Hs, Tp), label=r'ISSC Bretschneider, $H_s={0}m$, $T_p={1}s$'.format(Hs, Tp), **plotOptions(2, useColor))
    plot(W, psdJonswap(W, Hs, Tp, gamma = 2), label=r'JONSWAP, $H_s={0}m$, $T_p={1}s$, $\gamma=2$'.format(Hs, Tp), **plotOptions(3, useColor))
    plot(W, psdOchi(W, Hs, Tp, 1.5), label=r'Ochi, $H_s={0}m$, $T_p={1}s$, $\lambda=1.5$'.format(Hs, Tp), **plotOptions(4, useColor))
    plot(W, psdVignatBovis(W, Hs, Tp), label=r'Vignat-Bovis, $H_s={0}m$, $T_p={1}s$'.format(Hs, Tp), **plotOptions(5, useColor))
    plot(W, psdOneraTILV10(W, Hs, Tp), label=r'Onera TILV10, $H_s={0}m$, $T_p={1}s$'.format(Hs, Tp), **plotOptions(6, useColor))
    plot(W, psdGaussian(W, Hs, Tp, 0.02), label=r'Gauss, $H_s={0}m$, $T_p={1}s$, $\sigma=0.02$'.format(Hs, Tp), **plotOptions(7, useColor))
    legend(loc = 'upper right')
    xlabel(r'$\omega$ (rad/s)')
    ylabel(r'Power Spectral Density ($m^2.s$)')
    title(r'Spectra comparison')
    [savefig(f) for f in generateFigureFileNames('spectrumComparison1', extensions)]
    matplotlib.pyplot.close(fig)


def imageComparison2(Hs=1, Tp=10, extensions=getDefaultExtensions(), useColor=False):
    def plotOptions(n = 0, useColor = False):
        d = [{'c': 'k', 'linestyle': '-', 'marker': 'x', 'markevery': 11},
             {'c': 'k', 'linestyle': '-', 'marker': 's', 'markevery': 13},
             {'c': 'k', 'linestyle': '-', 'marker': 'D', 'markevery': 17}]
        opt = d[n % len(d)]
        if not useColor: opt['c'] = 'k'
        return opt
    dw = 0.01
    W = np.arange(dw, 4 + dw, dw)
    fig = figure()
    grid()
    plot(W, psdBretschneider(W, Hs, Tp), label=r'ISSC Bretschneider, $H_s={0}m$, $T_p={1}s$'.format(Hs, Tp), **plotOptions(0, useColor))
    plot(W, psdVignatBovis(W, Hs, Tp), label=r'Vignat-Bovis, $H_s={0}m$, $T_p={1}s$'.format(Hs, Tp), **plotOptions(1, useColor))
    plot(W, psdOneraTILV10(W, Hs, Tp), label=r'Onera TILV10, $H_s={0}m$, $T_p={1}s$'.format(Hs, Tp), **plotOptions(2, useColor))
    legend(loc = 'upper right')
    xlabel(r'$\omega$ (rad/s)')
    ylabel(r'Power Spectral Density ($m^2.s$)')
    [savefig(f) for f in generateFigureFileNames('spectrumComparison2', extensions)]
    matplotlib.pyplot.close(fig)


def generateImages(e, useColor=False):
    imagePiersonMoskowitz(extensions=e, useColor=useColor)
    imageJonswap(extensions=e, useColor=useColor)
    imageOchi(extensions=e, useColor=useColor)
    imageBretschneider(extensions=e, useColor=useColor)
    imageITTC(extensions=e, useColor=useColor)
    imageVignatBovis(extensions=e, useColor=useColor)
    imageOneraTILV10(extensions=e, useColor=useColor)
    imageGaussian(extensions=e, useColor=useColor)
    imageComparison1(extensions=e, useColor=useColor)
    imageComparison2(extensions=e, useColor=useColor)
    imageMonochromatique(extensions=e, useColor=useColor)
    imageBichromatique(extensions=e, useColor=useColor)


def getListOfPossibleImages():
    return ['PiersonMoskowitz', 'Jonswap', 'Ochi',
            'Bretschneider', 'ITTC', 'VignatBovis', 'OneraTILV10', 'Gaussian',
            'Comparison1', 'Comparison2', 'Monochromatique', 'Bichromatique']


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description = 'Script to generate different images.')
    parser.add_argument("-c", "--color", action = "store_true",
                        help = "Option to use color. Default is false",
                        default = False)
    args = parser.parse_args()
    generateImages('png', args.color)
    generateImages('svg', args.color)
