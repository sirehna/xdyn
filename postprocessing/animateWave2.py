# -*- coding: utf-8 -*-
#
# To be run directly from ParaView or with program pvpython from cli
#
# Remember to change the current working directory before running the script.
# To do so, use command os.chdir('') form package os.
#
# Use a mencoder command to create a video
# mencoder "mf://Im*.png" -mf fps=10:type=png -ovc lavc -lavcopts vcodec=mpeg4 -o Video.avi
#
# Requires:
# - A 3D mesh given as a VTK or a STL file
# - A CSV result file containing the displacement of the mobile to represent
# - A set of XYZ CSV files to represent wave elevation for each time steps. No
#   header are required
#
try: paraview.simple
except: from paraview.simple import *
paraview.simple._DisableFirstRenderCameraReset()
import argparse
import glob
import os
import math

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

def globalViewSettings(rv):
    rv.ViewSize = [1200, 800]
    rv.OrientationAxesVisibility = 0
    rv.CenterAxesVisibility = 0
    rv.LightIntensity = 0.15
    rv.LightSwitch = 0
    rv.UseOffscreenRendering = 1
    rv.UseOffscreenRenderingForScreenshots = 1
    rv.UseLight = 1
    rv.CameraParallelProjection = 1

def main(wavePathPattern = '/home/cady/simulator/code/build/executables/test/*.csv'):
	workingDirectory = os.getcwd()
	waveColor = [0.3333333333333333, 0.6666666666666666, 1.0]
	if not os.path.isabs(wavePathPattern):	
		wavePathPattern = os.path.join(workingDirectory,wavePathPattern)
	waveFiles = glob.glob(wavePathPattern)
	resultDirectory = r'Results'

	if not os.path.isabs(resultDirectory):
	    resultDirectory = os.path.join(os.getcwd(),resultDirectory)
	if not os.path.isdir(resultDirectory):
	    os.mkdir(resultDirectory)

	Res = CSVReader(FileName = waveFiles)
	Res.HaveHeaders = 0
	TableToPoints1 = TableToPoints()
	TableToPoints1.XColumn = 'Field 0'
	TableToPoints1.YColumn = 'Field 1'
	TableToPoints1.ZColumn = 'Field 2'
	delaunay2D = Delaunay2D()
	delaunay2DRepresentation = Show()
	delaunay2DRepresentation.ScaleFactor = 10.0
	delaunay2DRepresentation.EdgeColor = [0.0, 0.0, 0.5]
	delaunay2DRepresentation.DiffuseColor = waveColor

	renderView = GetRenderView()
	renderView.CompressorConfig = 'vtkSquirtCompressor 0 3'
	renderView.UseLight = 1
	renderView.LightSwitch = 0
	renderView.OrientationAxesInteractivity = 1
	renderView.OrientationAxesOutlineColor = [0.0, 1.0, 1.0]
	renderView.RemoteRenderThreshold = 3.0
	renderView.Background = [0.34, 0.34, 0.43]
	renderView.CenterAxesVisibility = 0
	renderView.CenterOfRotation = [50.0, 50.0, 0.07]
	renderView.CameraParallelProjection = 1
	renderView.CameraViewUp = [0.03, -0.45, -0.89]
	renderView.CameraPosition = [-130, 230, -130]
	renderView.CameraFocalPoint = [-4.0, 14.0, -7.7]
	renderView.CameraParallelScale = 49
	renderView.CameraClippingRange = [34.0, 610.0]

	animationScene = GetAnimationScene()
	animationScene.EndTime = len(waveFiles)
	animationScene.PlayMode = 'Snap To TimeSteps'
	animationScene.ViewModules = renderView
	Render()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description = 'Convert YAML output wave files to CSV')
    parser.add_argument("-c", "--csv",
                        help = "Name of the YAML files to process",
                        nargs = '*',
                        default = r'wave.yaml')
    args = parser.parse_args()
    main(args.csv[0])

