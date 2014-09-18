#!/usr/bin/env python

from pylab import linspace, pi, exp, real, imag

import matplotlib.path as mpath
import matplotlib.patches as mpatches
import matplotlib.pyplot as plt

from numpy import meshgrid, arange, polyval

n = 200
w = exp(1j * linspace(0, 2 * pi, n))

Path = mpath.Path

rk4_coeffs = [1 / 24., 1 / 6., 1 / 2., 1, 1]
rkck_coeffs = [7 / 864., 1. / 120, 1 / 24., 1 / 6., 1 / 2., 1, 1]

def flatten(list_of_lists):
    """ Transform a list of lists to a list whose elements are the union of all
        elements in the sublist.
    """
    return [item for sublist in list_of_lists for item in sublist]

def plot_complex(z):
    def plot_complex_(ax, z):
        verts = map(lambda z:(real(z), imag(z)), z)
        codes = [Path.MOVETO] + [Path.LINETO] * (len(verts) - 2) + [Path.CLOSEPOLY]

        path = mpath.Path(verts, codes)
        patch = mpatches.PathPatch(path, facecolor = [1, 0.5, 0.8], edgecolor = 'black', alpha = 1)
        ax.add_patch(patch)
    fig = plt.figure()
    fig.clf()
    ax = fig.add_subplot(111, aspect = 'equal')
    map(lambda x:plot_complex_(ax, x), z)
    ax.plot([-3, 2], [0, 0], '--', color = 'black')
    ax.plot([0, 0], [-3, 3], '--', color = 'black')
    ax.set_xlim(-3, 2)
    ax.set_ylim(-3, 3)
    return fig

def thetas(n):
    return map(lambda x:2.*pi * x / n, range(1, n + 1))

def convert_list_of_pairs_to_complex(L):
    return map(lambda z: z[0] + 1j * z[1], L)

def get_stability_contour(coeffs):
    delta = 0.025
    x = arange(-4.0, 4.0, delta)
    y = arange(-4.0, 4.0, delta)
    X, Y = meshgrid(x, y)
    Z = map(lambda p:abs(polyval(coeffs, p[0] + 1j * p[1])), zip(X, Y))
    CS = plt.contour(X, Y, Z, levels = [1])
    return map(convert_list_of_pairs_to_complex, CS.allsegs[0])

rk4 = get_stability_contour(rk4_coeffs)
rkck = get_stability_contour(rkck_coeffs)

for name, z in zip(['euler', 'trapezoidal', 'AB2', 'runge_kutta_4', 'runge_kutta_cash_karp'], \
                   [ [w - 1], [[5j, -5 + 5j, -5 - 5j, -5j, 5j]], [2 * (w ** 2 - w) / (3 * w - 1)], rk4, rkck]):
    plot_complex(z).savefig("images/" + name + "_stability.svg", transparent = True)

