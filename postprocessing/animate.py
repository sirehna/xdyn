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
import numpy as np

################################################################################
workingDirectory = os.getcwd()
mobile3DFileName = 'anthineas.stl'
mobile3DScale = 1
mobile3DTranslate = [-9.355, 0, +3.21]
mobileSimulationCsvFiles = [r'anthineas_in_wavesBret.csv']
mobileSimulationObjectNames = ['Anthineas']

wavePathPattern = 'anthineas_in_wavesBret_computed\waves*.csv'
waveColor = [0.3333333333333333, 0.6666666666666666, 1.0]
waveFiles = glob.glob(os.path.join(workingDirectory,wavePathPattern))

resultDirectory = r'Results'

################################################################################
defaultColor = [[1.00, 0.00, 0.00],
                [0.00, 1.00, 0.00],
                [0.00, 0.00, 1.00],
                [0.75, 0.75, 0.00],
                [0.00, 0.75, 0.75],
                [0.75, 0.00, 0.75],
                [0.25, 0.75, 0.75],
                [0.75, 0.25, 0.75],
                [0.75, 0.25, 0.25],
                [0.00, 0.50, 0.00]]
################################################################################
def quat2ParaviewAngle(quaternion):
    def quat2ctm(quaternion):
        ctm = np.zeros((3, 3))
        Q00 = quaternion[0] * quaternion[0]
        Q11 = quaternion[1] * quaternion[1]
        Q22 = quaternion[2] * quaternion[2]
        Q33 = quaternion[3] * quaternion[3]
        Q01 = quaternion[0] * quaternion[1]
        Q02 = quaternion[0] * quaternion[2]
        Q03 = quaternion[0] * quaternion[3]
        Q12 = quaternion[1] * quaternion[2]
        Q13 = quaternion[1] * quaternion[3]
        Q23 = quaternion[2] * quaternion[3]
        ctm[0, 0] = Q00 + Q11 - Q22 - Q33
        ctm[0, 1] = 2.0 * (Q12 + Q03)
        ctm[0, 2] = 2.0 * (Q13 - Q02)
        ctm[1, 0] = 2.0 * (Q12 - Q03)
        ctm[1, 1] = Q00 - Q11 + Q22 - Q33
        ctm[1, 2] = 2.0 * (Q23 + Q01)
        ctm[2, 0] = 2.0 * (Q13 + Q02)
        ctm[2, 1] = 2.0 * (Q23 - Q01)
        ctm[2, 2] = Q00 - Q11 - Q22 + Q33
        return ctm

    def ctm2eul312(ctm):
        """
        param[in] ctm Coordinate Transformation Matrix
        return phi_theta_psi Euler angle 213
        \code
        mRotationPhi:matrix([1,0,0],[0,cos(phi),-sin(phi)],[0,+sin(phi),cos(phi)]);
        mRotationTheta:matrix([cos(theta),0,+sin(theta)],[0,1,0],[-sin(theta),0,cos(theta)]);
        mRotationPsi:matrix([cos(psi),-sin(psi),0],[+sin(psi),cos(psi),0],[0,0,1]);
        mRot:mRotationPsi.mRotationPhi.mRotationTheta;
        mCtm:transpose(mRot);
        trigreduce(subst(%pi/2,phi,mCtm));
        \endcode
        """
        eps = 1e-13
        hyp = math.hypot(ctm[1, 0], ctm[1, 1]) # cos(phi)
        phi = math.atan2(ctm[1, 2], hyp)
        if(hyp > eps):
            # not degenerated
            theta = math.atan2(-ctm[0, 2], ctm[2, 2])
            psi = math.atan2(-ctm[1, 0], ctm[1, 1])
        else:
            theta = 0
            psi = math.atan2(ctm[0, 1], ctm[0, 0])
        return [phi, theta, psi]
    ctm = quat2ctm(quaternion)
    eul312 = ctm2eul312(ctm)
    return [math.degrees(e) for e in eul312]

def getResultColumnNames(objName):
    return ['x(' + objName + ')', 'y(' + objName + ')', 'z(' + objName + ')', \
            'qr(' + objName + ')', 'qi(' + objName + ')', \
            'qj(' + objName + ')', 'qk(' + objName + ')']

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

if not os.path.isabs(resultDirectory):
    resultDirectory = os.path.join(os.getcwd(),resultDirectory)
if not os.path.isdir(resultDirectory):
    os.mkdir(resultDirectory)

if (type(mobile3DFileName) == str or type(mobile3DFileName) == unicode):
    mobile3DFileName = [mobile3DFileName] * len(mobileSimulationCsvFiles)

listOfColumns = ['t']
for objName in mobileSimulationObjectNames:
    listOfColumns.extend(getResultColumnNames(objName))

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

nResultFiles = len(mobileSimulationCsvFiles)
rowResults = []
for csvFile in mobileSimulationCsvFiles:
    if not os.path.isabs(csvFile):
        csvFile = os.path.join(workingDirectory,csvFile)
    rr = CSVReader(FileName = [csvFile])
    rowResults.append(servermanager.Fetch(rr, 0).GetRowData())
resultsLabelsDict = []
for rowResult in rowResults:
    resultLabelsDict = {}
    for ar in range(rowResult.GetNumberOfArrays()):
        name = rowResult.GetArrayName(ar)
        if name in listOfColumns:
            resultLabelsDict[name] = ar
    resultsLabelsDict.append(resultLabelsDict)

# Determine t0, tf, and number of frames from result files
t0 = rowResults[0].GetArray(resultsLabelsDict[0]['t']).GetRange()[0]
tf = rowResults[0].GetArray(resultsLabelsDict[0]['t']).GetRange()[1]
N = rowResults[0].GetArray(resultsLabelsDict[0]['t']).GetSize()
for i, rowResult in enumerate(rowResults):
    t0 = min(t0, rowResult.GetArray(resultsLabelsDict[i]['t']).GetRange()[0])
    tf = max(tf, rowResult.GetArray(resultsLabelsDict[i]['t']).GetRange()[1])
    N = max(N, rowResult.GetArray(resultsLabelsDict[i]['t']).GetSize())
print('Tstart {0:+06.2f} - Tend {1:+06.2f} - N =  {2}'.format(t0, tf, N))
animationScene.AnimationTime = t0
animationScene.StartTime = t0
animationScene.EndTime = tf
animationScene.NumberOfFrames = N
animationDuration = tf - t0



Mobiles = []
Transform1 = []
Transform2 = []
for i, rowResult in enumerate(rowResults):
    ext = os.path.splitext(mobile3DFileName[i])[1]
    if os.path.isabs(mobile3DFileName[i]):
        currentMobile3DFileName = mobile3DFileName[i]
    else:
        currentMobile3DFileName = os.path.join(workingDirectory,mobile3DFileName[i])
    if ext == '.vtk':
        Mobile = LegacyVTKReader(FileNames = [currentMobile3DFileName])
    elif ext == '.stl':
        Mobile = STLReader(FileNames = [currentMobile3DFileName])
    else:
        pass
    RenameSource("Mobile_" + str(i), Mobile)
    Mobile.UpdatePipeline()
    active_objects.source.SMProxy.InvokeEvent('UserEvent', 'HideWidget')
    Transform1.append(Transform(Transform = "Transform"))
    Transform1[-1].Transform = "Transform"
    Transform1[-1].Transform.Scale = [mobile3DScale, mobile3DScale, mobile3DScale]
    Transform1[-1].Transform.Translate = mobile3DTranslate
    active_objects.source.SMProxy.InvokeEvent('UserEvent', 'HideWidget')
    RenameSource("Mobile_offset"+str(i), Transform1[-1])
    Transform2.append(Transform(Transform = "Transform"))
    Transform2[-1].Transform = "Transform"
    active_objects.source.SMProxy.InvokeEvent('UserEvent', 'HideWidget')
    RenameSource("Mobile_tr"+str(i), Transform2[-1])
    SM_tr_repr = Show()
    if nResultFiles > 1:
        SM_tr_repr.DiffuseColor = defaultColor[i]
    Show()
    Render()
    RenderView = GetRenderView()
    Mobiles.append(Mobile)


timeLabel = AnnotateTimeFilter()
timeLabel_repr = Show()
timeLabel.Format = 'Time: %04.2f'
timeLabel_repr.FontSize = 14
timeLabel_repr.WindowLocation = 'UpperRightCorner'

makeOnlyImages = True
if makeOnlyImages:
    for i in range(N):
        animationScene.AnimationTime = i
        for j, (transform2, rowResult) in enumerate(zip(Transform2, rowResults)):
            col = getResultColumnNames(mobileSimulationObjectNames[0])
            xyz = [rowResult.GetArray(resultsLabelsDict[j][col[0]]).GetTuple(i)[0], \
                   rowResult.GetArray(resultsLabelsDict[j][col[1]]).GetTuple(i)[0], \
                   rowResult.GetArray(resultsLabelsDict[j][col[2]]).GetTuple(i)[0]]
            quat = [rowResult.GetArray(resultsLabelsDict[j][col[3]]).GetTuple(i)[0], \
                    rowResult.GetArray(resultsLabelsDict[j][col[4]]).GetTuple(i)[0], \
                    rowResult.GetArray(resultsLabelsDict[j][col[5]]).GetTuple(i)[0], \
                    rowResult.GetArray(resultsLabelsDict[j][col[6]]).GetTuple(i)[0]]
            par = quat2ParaviewAngle(quat)
            transform2.Transform.Translate = xyz
            transform2.Transform.Rotate = par
            transform2.UpdatePipeline()
        Render()
        paraviewSaveImage(imageFilename = os.path.join(resultDirectory, r'Im_{0:05d}.png'.format(i)), \
                          writeImageMagnification = 3)
else:
    ASMs_KF_x_Cue     = [GetAnimationTrack( 'Position', 0, proxy=ASM_tr.Transform ) for ASM_tr in Transform2]
    ASMs_KF_y_Cue     = [GetAnimationTrack( 'Position', 1, proxy=ASM_tr.Transform ) for ASM_tr in Transform2]
    ASMs_KF_z_Cue     = [GetAnimationTrack( 'Position', 2, proxy=ASM_tr.Transform ) for ASM_tr in Transform2]
    ASMs_KF_phi_Cue   = [GetAnimationTrack( 'Rotation', 0, proxy=ASM_tr.Transform ) for ASM_tr in Transform2]
    ASMs_KF_theta_Cue = [GetAnimationTrack( 'Rotation', 1, proxy=ASM_tr.Transform ) for ASM_tr in Transform2]
    ASMs_KF_psi_Cue   = [GetAnimationTrack( 'Rotation', 2, proxy=ASM_tr.Transform ) for ASM_tr in Transform2]

    ASMs_KF_x         = [[] for ASM_tr in Transform2]
    ASMs_KF_y         = [[] for ASM_tr in Transform2]
    ASMs_KF_z         = [[] for ASM_tr in Transform2]
    ASMs_KF_phi       = [[] for ASM_tr in Transform2]
    ASMs_KF_theta     = [[] for ASM_tr in Transform2]
    ASMs_KF_psi       = [[] for ASM_tr in Transform2]

    for j,rowResult in enumerate(rowResults):
        N = rowResult.GetNumberOfTuples()
        for i in range(0,N):
            animationScene.AnimationTime = i
            keytime = (rowResult.GetArray(resultsLabelsDict[j]['t']).GetTuple(i)[0]-t0)/animationDuration

            col = getResultColumnNames(mobileSimulationObjectNames[0])
            xyz = [rowResult.GetArray(resultsLabelsDict[j][col[0]]).GetTuple(i)[0], \
                   rowResult.GetArray(resultsLabelsDict[j][col[1]]).GetTuple(i)[0], \
                   rowResult.GetArray(resultsLabelsDict[j][col[2]]).GetTuple(i)[0]]
            quat = [rowResult.GetArray(resultsLabelsDict[j][col[3]]).GetTuple(i)[0], \
                    rowResult.GetArray(resultsLabelsDict[j][col[4]]).GetTuple(i)[0], \
                    rowResult.GetArray(resultsLabelsDict[j][col[5]]).GetTuple(i)[0], \
                    rowResult.GetArray(resultsLabelsDict[j][col[6]]).GetTuple(i)[0]]
            par = quat2ParaviewAngle(quat)
            ASMs_KF_x[j].append(    CompositeKeyFrame( KeyTime=keytime, KeyValues=[xyz[0]]))
            ASMs_KF_y[j].append(    CompositeKeyFrame( KeyTime=keytime, KeyValues=[xyz[1]]))
            ASMs_KF_z[j].append(    CompositeKeyFrame( KeyTime=keytime, KeyValues=[xyz[2]]))
            ASMs_KF_phi[j].append(  CompositeKeyFrame( KeyTime=keytime, KeyValues=[par[0]]))
            ASMs_KF_theta[j].append(CompositeKeyFrame( KeyTime=keytime, KeyValues=[par[1]]))
            ASMs_KF_psi[j].append(  CompositeKeyFrame( KeyTime=keytime, KeyValues=[par[2]]))
    for j in range(nResultFiles):
        ASMs_KF_x_Cue[j].KeyFrames     = ASMs_KF_x[j]
        ASMs_KF_y_Cue[j].KeyFrames     = ASMs_KF_y[j]
        ASMs_KF_z_Cue[j].KeyFrames     = ASMs_KF_z[j]
        ASMs_KF_phi_Cue[j].KeyFrames   = ASMs_KF_phi[j]
        ASMs_KF_theta_Cue[j].KeyFrames = ASMs_KF_theta[j]
        ASMs_KF_psi_Cue[j].KeyFrames   = ASMs_KF_psi[j]
    Render()
