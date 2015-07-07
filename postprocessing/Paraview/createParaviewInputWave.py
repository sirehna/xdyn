import os
import h5py
import numpy as np
from spt.vtk.vtkMesh import TriangulatedMeshGrid

def doesDictHasAllRequiredKeys(d, requiredKeys = ('X', 'Y', 'Z', 'Quat')):
    for k in requiredKeys:
        if k not in d.keys():
            return False
    return True

def createWaveSurfacicFileFromH5Waves(h5Filename, outputBaseName = None, verbose = False):
    if outputBaseName  is None or outputBaseName == '':
        outputDir = os.path.splitext(h5Filename)[0] + '_Waves'
        outputBaseName = os.path.splitext(os.path.basename(h5Filename))[0]
    else:
        outputDir = os.path.dirname(outputBaseName)
        outputBaseName = os.path.basename(outputBaseName)
    if verbose:
        print("Output directory {0}".format(outputDir))
        print("Output base file {0}".format(outputBaseName))
    if not os.path.isdir(outputDir):
        os.mkdir(outputDir)
    h = h5py.File(h5Filename, 'r')
    outputs = h['outputs']
    waves = outputs['waves']
    if not doesDictHasAllRequiredKeys(waves, requiredKeys = ('t', 'x', 'y', 'z')):
        raise Exception
    t, x, y, Z = waves['t'], waves['x'], waves['y'], waves['z']
    n = len(t)
    nx, ny = len(x[0, :]), len(y[0, :])
    T = TriangulatedMeshGrid(nx = nx, ny = ny)
    for i in range(n):
        if verbose :
            print("Dealing wave {0} xMin {1} xMax {2} yMin {3} yMax {4}".\
                  format(i, min(x[i, :]), max(x[i, :]), min(y[i, :]), max(y[i, :])))
        Y, X = np.meshgrid(y[i, :], x[i, :])
        T.updatePoints(X, Y, Z[:, :, i])
        T.export(os.path.join(outputDir, "{0}_{1:05d}.vtk".format(outputBaseName, i)), binary = True)
    h.close()

if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description = 'Generate ASCII files for Paraview visualization, and also optionnaly a Parview animation script')
    parser.add_argument("--hdf5",
                        help = "Name of the HDF5 output file",
                        default = '')
    parser.add_argument("-v", "--verbose",
                        action = "store_true",
                        default = False,
                        help = "Option used to debug")
    args = parser.parse_args()
    h5Filename = args.hdf5
    if h5Filename == '':
        raise IOError("Required input file should not be empty")
    if not os.path.exists(h5Filename):
        raise IOError("Required input file '{0}' does not exist".format(h5Filename))
    createWaveSurfacicFileFromH5Waves(args.hdf5, verbose = args.verbose)
