# -*- coding: utf-8 -*-
#
# To be run directly from ParaView or with program pvpython (Windows) or
# pvbatch (Linux) from cli
#
# Remember to change the current working directory before running the script.
# To do so, use command os.chdir('') form package os.
#
# Use a mencoder command to create a video
# mencoder "mf://Im*.png" -mf fps=10:type=png -ovc lavc -lavcopts vcodec=mpeg4 -o Video.avi
#
# Requires:
# - A set of XYZ CSV files to represent wave elevation for each time steps. No
#   header are required
#
# For each video, view settings should be tuned. Among parameters, these are the
# most important
#   rv = GetRenderView()
#   rv.CenterOfRotation
#   rv.CameraPosition
#   rv.CameraViewUp
#   rv.CameraFocalPoint
#   rv.CameraParallelScale
#   rv.CameraClippingRange
try: paraview.simple
except: from paraview.simple import *
paraview.simple._DisableFirstRenderCameraReset()
import argparse
import glob
import os
import re

################################################################################
def paraviewSaveImage(\
        imageFilename = r'D:/AZERTY_0000.png', \
        writeImageMagnification = 2):
    rv = GetRenderView()
    back_save = rv.Background
    rv.Background = [1.0, 1.0, 1.0]
    # imagePath = join(SNAPSHOTS_PATH,reprTitle+'_'+viewTitle+'.png')
    # WriteImage(imagePath)
    WriteImage(imageFilename, view = rv, Magnification = writeImageMagnification)
    rv = GetRenderView()
    rv.Background = [0.32941176470588235, 0.34901960784313724, 0.42745098039215684]
    Render()

def globalViewSettings(rv = GetRenderView(), offscreenRendering = False):
    rv.ViewSize = [1600, 900]
    rv.OrientationAxesVisibility = 0
    rv.CenterAxesVisibility = 0
    rv.LightIntensity = 0.15
    rv.LightSwitch = 0
    if offscreenRendering:
        rv.UseOffscreenRendering = 1
        rv.UseOffscreenRenderingForScreenshots = 1
    rv.UseLight = 1
    rv.CameraParallelProjection = 1
    rv.CompressorConfig = 'vtkSquirtCompressor 0 3'
    rv.OrientationAxesInteractivity = 1
    rv.OrientationAxesOutlineColor = [0.0, 1.0, 1.0]
    rv.RemoteRenderThreshold = 3.0
    rv.Background = [0.34, 0.34, 0.43]
    # Option to update
    rv.CenterOfRotation = [50.0, 50.0, 0.07]
    rv.CameraPosition = [-41.0, 103.0, -49.5]
    rv.CameraViewUp = [0.10, -0.4, -0.9]
    rv.CameraFocalPoint = [0.69, -0.11, 0.37]
    rv.CameraParallelScale = 18
    rv.CameraClippingRange = [81.0, 173.0]
    return rv

def main(\
        wavePathPattern = r'/home/cady/simulator/code/build/executables/test/*.csv', \
        resultDirectory = r'Results',
        saveImages = False,
        debug = False):
    workingDirectory = os.getcwd()
    waveColor = [1.0 / 3.0, 2.0 / 3.0, 1.0]
    if re.search("\*\.[Cc][Ss][Vv]$", wavePathPattern):
        waveDirectory = wavePathPattern[:-6]
        extension = wavePathPattern[-5:]
    else:
        waveDirectory = wavePathPattern
        extension = '*.csv'
    if not os.path.isabs(waveDirectory):
        waveDirectory = os.path.join(workingDirectory, waveDirectory)
    waveFiles = sorted(glob.glob(os.path.join(wavePathPattern, extension)))
    if not os.path.isabs(resultDirectory):
        if not os.path.isabs(waveDirectory):
            resultDirectory = os.path.join(waveDirectory, resultDirectory)
        else:
            resultDirectory = os.path.join(waveDirectory, resultDirectory)
    if not os.path.isdir(resultDirectory):
        os.mkdir(resultDirectory)
    if debug:
        print(extension)
        print(waveFiles)
    Res = CSVReader(FileName = waveFiles)
    Res.HaveHeaders = 0
    tableToPoints = TableToPoints()
    tableToPoints.XColumn = 'Field 0'
    tableToPoints.YColumn = 'Field 1'
    tableToPoints.ZColumn = 'Field 2'
    delaunay2D = Delaunay2D()
    delaunay2DRepresentation = Show()
    delaunay2DRepresentation.ScaleFactor = 10.0
    delaunay2DRepresentation.EdgeColor = [0.0, 0.0, 0.5]
    delaunay2DRepresentation.DiffuseColor = waveColor

    renderView = globalViewSettings()

    animationScene = GetAnimationScene()
    animationScene.EndTime = len(waveFiles)
    animationScene.PlayMode = 'Snap To TimeSteps'
    animationScene.ViewModules = renderView
    Render()
    if saveImages:
        for i in range(len(waveFiles)):
            animationScene.AnimationTime = i
            Render()
            paraviewSaveImage(imageFilename = os.path.join(resultDirectory, r'Im_{0:05d}.png'.format(i)), \
                              writeImageMagnification = 3)

defaultPath = os.getcwd()
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description = 'Loads in Paraview a set of CSV wave elevation files')
    parser.add_argument("-c", "--csv",
                        help = "Directory containing CSV wave elevation files",
                        nargs = 1,
                        default = [defaultPath])
    parser.add_argument("-s", "--saveImages",
                        action = "store_true",
                        default = True,
                        help = "Option used to save images")
    parser.add_argument("-d", "--debug",
                        action = "store_true",
                        default = False,
                        help = "Option used to debug")
    args = parser.parse_args()
    main(args.csv[0], resultDirectory = r'Results', saveImages = args.saveImages, debug = args.debug)
elif __name__ == "__vtkconsole__":
    main(defaultPath, resultDirectory = r'Results', saveImages = False)

