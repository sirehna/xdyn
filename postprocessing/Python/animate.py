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
# For each video, view settings should be tuned. Among parameters, these are the
# most important
#   rv = GetRenderView()
#   rv.CenterOfRotation
#   rv.CameraPosition
#   rv.CameraViewUp
#   rv.CameraFocalPoint
#   rv.CameraParallelScale
#   rv.CameraClippingRange
#
# Limitations:
# - All simulations should have the same time basis

try: paraview.simple
except: from paraview.simple import *
paraview.simple._DisableFirstRenderCameraReset()
import argparse
import glob
import os
import math
import numpy as np
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

def main(mobile, wave, resultDirectory = r'Results', \
    saveImages = False, makeOnlyImages = False):
    if makeOnlyImages:
        saveImages = True
    workingDirectory = os.getcwd()
    mobile3DFileName = mobile['3DFileName']
    mobile3DScale = mobile['3DScale']
    mobile3DTranslate = mobile['3DTranslate']
    mobileSimulationCsvFiles = mobile['SimulationCsvFiles']
    mobileSimulationObjectNames = mobile['SimulationObjectNames']

    waveColor = wave['Color']
    waveFiles = wave['Files']

    if not os.path.isabs(resultDirectory):
        resultDirectory = os.path.join(os.getcwd(), resultDirectory)
    if not os.path.isdir(resultDirectory):
        os.mkdir(resultDirectory)

    if (type(mobile3DFileName) == str or type(mobile3DFileName) == unicode):
        mobile3DFileName = [mobile3DFileName] * len(mobileSimulationCsvFiles)

    listOfColumns = ['t']
    for objName in mobileSimulationObjectNames:
        listOfColumns.extend(getResultColumnNames(objName))

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
    # animationScene.PlayMode = 'Sequence'
    animationScene.ViewModules = renderView

    nResultFiles = len(mobileSimulationCsvFiles)
    rowResults = []
    for csvFile in mobileSimulationCsvFiles:
        if not os.path.isabs(csvFile):
            csvFile = os.path.join(workingDirectory, csvFile)
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
    if animationScene.PlayMode == 'Snap To TimeSteps':
        animationScene.AnimationTime = 0
        animationScene.StartTime = 0
        animationScene.EndTime = N - 1
    else:
        animationScene.AnimationTime = t0
        animationScene.StartTime = t0
        animationScene.EndTime = tf
    animationScene.NumberOfFrames = N
    animationDuration = tf - t0

    Mobiles = []
    Mobile_offset = []
    Mobile_tr = []
    for i, rowResult in enumerate(rowResults):
        ext = os.path.splitext(mobile3DFileName[i])[1]
        if os.path.isabs(mobile3DFileName[i]):
            currentMobile3DFileName = mobile3DFileName[i]
        else:
            currentMobile3DFileName = os.path.join(workingDirectory, mobile3DFileName[i])
        if ext == '.vtk':
            Mobile = LegacyVTKReader(FileNames = [currentMobile3DFileName])
        elif ext == '.stl':
            Mobile = STLReader(FileNames = [currentMobile3DFileName])
        else:
            pass
        RenameSource("Mobile_" + str(i), Mobile)
        Mobile.UpdatePipeline()
        active_objects.source.SMProxy.InvokeEvent('UserEvent', 'HideWidget')
        Mobile_offset.append(Transform(Transform = "Transform"))
        Mobile_offset[-1].Transform = "Transform"
        Mobile_offset[-1].Transform.Scale = [mobile3DScale[i]] * 3
        Mobile_offset[-1].Transform.Translate = mobile3DTranslate[i]
        active_objects.source.SMProxy.InvokeEvent('UserEvent', 'HideWidget')
        RenameSource("Mobile_offset" + str(i), Mobile_offset[-1])
        Mobile_tr.append(Transform(Transform = "Transform"))
        Mobile_tr[-1].Transform = "Transform"
        active_objects.source.SMProxy.InvokeEvent('UserEvent', 'HideWidget')
        RenameSource("Mobile_tr" + str(i), Mobile_tr[-1])
        SM_tr_repr = Show()
        if nResultFiles > 1:
            SM_tr_repr.DiffuseColor = defaultColor[i]
        Show()
        Render()
        RenderView = GetRenderView()
        Mobiles.append(Mobile)

    timeLabel = AnnotateTimeFilter()
    timeLabel_repr = Show()
    timeLabel.Format = 'Time: %06.2fs'
    timeLabel.Scale = animationDuration / float(N - 1)
    timeLabel_repr.FontSize = 14
    timeLabel_repr.Color = [0.0, 0.0, 0.0]
    timeLabel_repr.WindowLocation = 'UpperRightCorner'

    if makeOnlyImages:
        for i in range(N):
            animationScene.AnimationTime = i
            for j, (mobile_tr, rowResult) in enumerate(zip(Mobile_tr, rowResults)):
                col = getResultColumnNames(mobileSimulationObjectNames[0])
                xyz = [rowResult.GetArray(resultsLabelsDict[j][col[0]]).GetTuple(i)[0], \
                       rowResult.GetArray(resultsLabelsDict[j][col[1]]).GetTuple(i)[0], \
                       rowResult.GetArray(resultsLabelsDict[j][col[2]]).GetTuple(i)[0]]
                quat = [rowResult.GetArray(resultsLabelsDict[j][col[3]]).GetTuple(i)[0], \
                        rowResult.GetArray(resultsLabelsDict[j][col[4]]).GetTuple(i)[0], \
                        rowResult.GetArray(resultsLabelsDict[j][col[5]]).GetTuple(i)[0], \
                        rowResult.GetArray(resultsLabelsDict[j][col[6]]).GetTuple(i)[0]]
                par = quat2ParaviewAngle(quat)
                mobile_tr.Transform.Translate = xyz
                mobile_tr.Transform.Rotate = par
                mobile_tr.UpdatePipeline()
            Render()
            paraviewSaveImage(imageFilename = os.path.join(resultDirectory, r'Im_{0:05d}.png'.format(i)), \
                              writeImageMagnification = 3)
    else:
        Mobile_KF_Tx_Cue = [GetAnimationTrack('Position', 0, proxy = mobile_tr.Transform) for mobile_tr in Mobile_tr]
        Mobile_KF_Ty_Cue = [GetAnimationTrack('Position', 1, proxy = mobile_tr.Transform) for mobile_tr in Mobile_tr]
        Mobile_KF_Tz_Cue = [GetAnimationTrack('Position', 2, proxy = mobile_tr.Transform) for mobile_tr in Mobile_tr]
        Mobile_KF_Rx_Cue = [GetAnimationTrack('Rotation', 0, proxy = mobile_tr.Transform) for mobile_tr in Mobile_tr]
        Mobile_KF_Ry_Cue = [GetAnimationTrack('Rotation', 1, proxy = mobile_tr.Transform) for mobile_tr in Mobile_tr]
        Mobile_KF_Rz_Cue = [GetAnimationTrack('Rotation', 2, proxy = mobile_tr.Transform) for mobile_tr in Mobile_tr]

        Mobile_KF_Tx = [[] for _ in Mobile_tr]
        Mobile_KF_Ty = [[] for _ in Mobile_tr]
        Mobile_KF_Tz = [[] for _ in Mobile_tr]
        Mobile_KF_Rx = [[] for _ in Mobile_tr]
        Mobile_KF_Ry = [[] for _ in Mobile_tr]
        Mobile_KF_Rz = [[] for _ in Mobile_tr]

        for j, rowResult in enumerate(rowResults):
            for i in range(0, N):
                if animationScene.PlayMode == 'Snap To TimeSteps':
                    keytime = float(i) / float(N)
                else:
                    keytime = (rowResult.GetArray(resultsLabelsDict[j]['t']).GetTuple(i)[0] - t0) / animationDuration
                col = getResultColumnNames(mobileSimulationObjectNames[j])
                xyz = [rowResult.GetArray(resultsLabelsDict[j][col[0]]).GetTuple(i)[0], \
                       rowResult.GetArray(resultsLabelsDict[j][col[1]]).GetTuple(i)[0], \
                       rowResult.GetArray(resultsLabelsDict[j][col[2]]).GetTuple(i)[0]]
                quat = [rowResult.GetArray(resultsLabelsDict[j][col[3]]).GetTuple(i)[0], \
                        rowResult.GetArray(resultsLabelsDict[j][col[4]]).GetTuple(i)[0], \
                        rowResult.GetArray(resultsLabelsDict[j][col[5]]).GetTuple(i)[0], \
                        rowResult.GetArray(resultsLabelsDict[j][col[6]]).GetTuple(i)[0]]
                par = quat2ParaviewAngle(quat)
                Mobile_KF_Tx[j].append(CompositeKeyFrame(KeyTime = keytime, KeyValues = [xyz[0]]))
                Mobile_KF_Ty[j].append(CompositeKeyFrame(KeyTime = keytime, KeyValues = [xyz[1]]))
                Mobile_KF_Tz[j].append(CompositeKeyFrame(KeyTime = keytime, KeyValues = [xyz[2]]))
                Mobile_KF_Rx[j].append(CompositeKeyFrame(KeyTime = keytime, KeyValues = [par[0]]))
                Mobile_KF_Ry[j].append(CompositeKeyFrame(KeyTime = keytime, KeyValues = [par[1]]))
                Mobile_KF_Rz[j].append(CompositeKeyFrame(KeyTime = keytime, KeyValues = [par[2]]))
        for j in range(nResultFiles):
            Mobile_KF_Tx_Cue[j].KeyFrames = Mobile_KF_Tx[j]
            Mobile_KF_Ty_Cue[j].KeyFrames = Mobile_KF_Ty[j]
            Mobile_KF_Tz_Cue[j].KeyFrames = Mobile_KF_Tz[j]
            Mobile_KF_Rx_Cue[j].KeyFrames = Mobile_KF_Rx[j]
            Mobile_KF_Ry_Cue[j].KeyFrames = Mobile_KF_Ry[j]
            Mobile_KF_Rz_Cue[j].KeyFrames = Mobile_KF_Rz[j]
        Render()
        if saveImages:
            for i in range(N):
                animationScene.AnimationTime = i
                Render()
                paraviewSaveImage(\
                        imageFilename = \
                        os.path.join(resultDirectory, r'Im_{0:05d}.png'.format(i)), \
                        writeImageMagnification = 3)


def defaultParameters01():
    # Default data to represent one ship on waves
    workingDirectory = os.getcwd()
    mobile = {}
    mobile['3DFileName'] = ['anthineas.stl']
    mobile['3DScale'] = [1]
    mobile['3DTranslate'] = [[-9.355, 0, +3.21]]
    mobile['SimulationCsvFiles'] = [r'anthineas.csv']
    mobile['SimulationObjectNames'] = ['Anthineas']

    wave = {}
    wave['PathPattern'] = 'waves\waves*.csv'
    wave['Color'] = [1.0 / 3.0, 2.0 / 3.0, 1.0]
    wave['Files'] = sorted(glob.glob(os.path.join(workingDirectory, wave['PathPattern'])))

    resultDirectory = r'Results'

    saveImages = True
    return mobile, wave, resultDirectory, saveImages

def defaultParameters02():
    # Default data to represent several ships on waves
    workingDirectory = os.getcwd()
    mobile = {}
    mobile['3DFileName'] = ['anthineas.stl'] * 2
    mobile['3DScale'] = [1] * 2
    mobile['3DTranslate'] = [[-9.355, 0, +3.21]] * 2
    mobile['SimulationCsvFiles'] = [r'anthineas_1.csv', r'anthineas_2.csv']
    mobile['SimulationObjectNames'] = ['Anthineas'] * 2

    wave = {}
    wave['PathPattern'] = 'waves\waves*.csv'
    wave['Color'] = [1.0 / 3.0, 2.0 / 3.0, 1.0]
    wave['Files'] = sorted(glob.glob(os.path.join(workingDirectory, wave['PathPattern'])))

    resultDirectory = r'Results'

    saveImages = True
    return mobile, wave, resultDirectory, saveImages

mobile, wave, resultDirectory, saveImages = defaultParameters01()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description = 'Loads in Paraview a set of CSV wave elevation files')
    parser.add_argument("-w", "--waves",
                        help = "Directory containing CSV wave elevation files. Default is {0}".format(wave['PathPattern']),
                        nargs = 1,
                        default = [wave['PathPattern']])
    parser.add_argument("-s", "--saveImages",
                        action = "store_true",
                        default = True,
                        help = "Option used to save images")
    parser.add_argument("-o", "--outputDirectory", type = str,
                        default = 'Results',
                        help = "Name of the directory that will contain result images")
    args = parser.parse_args()
    wave['PathPattern'] = args.waves[0]
    saveImages = args.saveImages
    outputDirectory = args.outputDirectory
    main(mobile, wave, resultDirectory = outputDirectory, saveImages = saveImages)
elif __name__ == "__vtkconsole__":
    main(mobile, wave, resultDirectory, saveImages)
