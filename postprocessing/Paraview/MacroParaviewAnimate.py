# Jinja template
from paraview.simple import *
paraview.simple._DisableFirstRenderCameraReset()

import os.path as op
from math import pi
from glob import glob

defaultColor = [[1.00,0.00,0.00],
                [0.00,1.00,0.00],
                [0.00,0.00,1.00],
                [0.75,0.75,0.00],
                [0.00,0.75,0.75],
                [0.75,0.00,0.75],
                [0.25,0.75,0.75],
                [0.75,0.25,0.75],
                [0.75,0.25,0.25],
                [0.00,0.50,0.00]]

# ======================================================================
#   USER INPUTS
# ======================================================================

offsets = {{ offsets }}

simulationDirectory = r'{{ simulationDirectory }}'
SM_path             = {{ SM_path }}
resultFiles         = {{ resultFiles }}
resultLegend        = {{ resultLegend }}
waveDirectory       = r'{{ waveDirectory }}'

# ======================================================================
#   END USER INPUTS
# ======================================================================

if waveDirectory != '':
    waveDirectory = op.join(simulationDirectory, waveDirectory)
    if op.isdir(waveDirectory):
        files = sorted(glob(op.join(waveDirectory, '*.vtk')))
        waves = LegacyVTKReader(FileNames=files)
        renderView = GetActiveViewOrCreate('RenderView')
        wavesDisplay = Show(waves, renderView)
        wavesDisplay.ColorArrayName = [None, '']
        Hide3DWidgets(proxy = waves)

if resultLegend:
    if (type(resultLegend)==str or type(resultLegend)==unicode):
        resultLegend = [resultLegend]
if (type(resultFiles)==str or type(resultFiles)==unicode):
    resultFiles = [resultFiles]
resultFiles = [op.join(simulationDirectory,resultFile) for resultFile in resultFiles]
nResultFiles = len(resultFiles)

resultFilesTraj = [op.splitext(resultFile)[0]+r'.vtk' for resultFile in resultFiles]
resultFilesTraj = [resultFile for resultFile in resultFilesTraj if op.exists(resultFile)]

# ======================================================================
#   END OF USER INPUTS
# ======================================================================
SMsTraj = []
for i in range(nResultFiles):
    SMsTraj.append(LegacyVTKReader(FileNames=[resultFilesTraj[i]]))
    RenameSource("SM_Traj"+str(i), SMsTraj[-1])
    SMsTraj[-1].UpdatePipeline()
    SM_Traj_repr = Show()
    SM_Traj_repr.EdgeColor = [0.0, 0.0, 0.50]
    if nResultFiles>1:
        SM_Traj_repr.DiffuseColor = defaultColor[i]

SMs = []
for i in range(nResultFiles):
    if op.splitext(SM_path[i])[1]=='.stl':
        SMs.append(STLReader(FileNames=[SM_path[i]]))
    elif op.splitext(SM_path[i])[1]=='.vtk':
        SMs.append(LegacyVTKReader(FileNames=[SM_path[i]]))
    else:
        raise Exception
    RenameSource("SM_"+str(i), SMs[-1])
    SMs[-1].UpdatePipeline()
    RenderView = GetRenderView()
    SM_repr = Show()
    SM_repr.EdgeColor = [0.0, 0.0, 0.50]
    SM_repr.Visibility = 0
    RenderView = GetRenderView()

RenderView = GetRenderView()
RenderView.CameraViewUp            = [-0.38, -0.28, -0.87]
RenderView.CameraFocalPoint        = [19, 9, -15]
RenderView.CenterAxesVisibility    = 0
RenderView.CameraClippingRange     = [38.3, 181.5]
RenderView.CameraPosition          = [79, 34, -50]

Results     = []
datas       = []
rowResults  = []
for i,resultFile in enumerate(resultFiles):
    Results.append(CSVReader( FileName=resultFile))
    RenameSource("Result_"+str(i), Results[-1])
    Results[-1].UpdatePipeline()
    datas.append(servermanager.Fetch(Results[-1],0))
    rowResults.append(datas[-1].GetRowData())

labelsList = ['t','X','Y','Z','ParaviewRotDegX','ParaviewRotDegY','ParaviewRotDegZ']
resultsLabelsDict = []
for rowResult in rowResults:
    resultLabelsDict = {}
    for ar in range(rowResult.GetNumberOfArrays()):
        name = rowResult.GetArrayName(ar)
        if name in labelsList:
            resultLabelsDict[name] = ar
    resultsLabelsDict.append(resultLabelsDict)

SMs_tr_Initials = []
for i,SM in enumerate(SMs):
    offset = offsets[resultLegend[i]]
    SetActiveSource(SM)
    SMs_tr_Initials.append(Transform(Transform="Transform"))
    SMs_tr_Initials[-1].Transform = "Transform"
    SMs_tr_Initials[-1].Transform.Translate = [offset['X'],offset['Y'],offset['Z']]
    SMs_tr_Initials[-1].Transform.Rotate    = [offset['RotX'],offset['RotY'],offset['RotZ']]
    RenameSource("SM_tr_Init"+str(i), SMs_tr_Initials[-1])
    SMs_tr_Initials[-1].UpdatePipeline()
    RenderView = GetRenderView()
    SMs_tr_Initials_repr = Show()
    SMs_tr_Initials_repr.EdgeColor = [0.0, 0.0, 0.0]
    SMs_tr_Initials_repr.Visibility = 0
    RenderView = GetRenderView()
    # Hide(SMs_tr_Initials[-1], RenderView)

SMs_tr = []
for i,SM in enumerate(SMs_tr_Initials):
    SetActiveSource(SM)
    SMs_tr.append(Transform( Transform="Transform"))
    SMs_tr[-1].Transform = "Transform"
    RenameSource("SM_tr"+str(i), SMs_tr[-1])
    SMs_tr[-1].UpdatePipeline()
    RenderView = GetRenderView()
    SM_tr_repr = Show()
    SM_tr_repr.EdgeColor = [0.0, 0.0, 0.50]
    if nResultFiles>1:
        SM_tr_repr.DiffuseColor = defaultColor[i]
    SetActiveSource(SM)

t0 = rowResults[0].GetArray(resultsLabelsDict[0]['t']).GetRange()[0]
tf = rowResults[0].GetArray(resultsLabelsDict[0]['t']).GetRange()[1]
N  = rowResults[0].GetArray(resultsLabelsDict[0]['t']).GetSize()
for i,rowResult in enumerate(rowResults):
    t0 = min(t0,rowResult.GetArray(resultsLabelsDict[i]['t']).GetRange()[0])
    tf = max(tf,rowResult.GetArray(resultsLabelsDict[i]['t']).GetRange()[1])
    N  = max(N,rowResult.GetArray(resultsLabelsDict[i]['t']).GetSize())

print('Tstart {0:+06.2f} - Tend {1:+06.2f} - N =  {2}'.format(t0, tf,N))

TimeAnimationCue1 = GetTimeTrack()
scene = GetAnimationScene()
#scene.PlayMode = 'Snap To TimeSteps'
scene.PlayMode = 'Sequence'
scene.AnimationTime = t0
scene.StartTime = t0
scene.EndTime = tf
scene.NumberOfFrames = N
animationDuration = tf - t0

Render()

pos = ('TX','TY','TZ')
rot = ('RX','RY','RZ')

SMs_KF_Cue = {k:[None]*nResultFiles for k in pos+rot}
SMs_KF     = {k:[[]]*nResultFiles for k in pos+rot}

for i,SM_tr in enumerate(SMs_tr):
    for ii,k in enumerate(pos):
        SMs_KF_Cue[k][i] = GetAnimationTrack( 'Position', ii, proxy = SM_tr.Transform )
    for ii,k in enumerate(rot):
        SMs_KF_Cue[k][i] = GetAnimationTrack( 'Rotation', ii, proxy = SM_tr.Transform )

for j,rowResult in enumerate(rowResults):
    N = rowResult.GetNumberOfTuples()
    for ii in range(0,N,1):
        keytime = (rowResult.GetArray(resultsLabelsDict[j]['t']).GetTuple(ii)[0]-t0)/animationDuration
        for t,u in zip(pos+rot,labelsList[1:]):
            SMs_KF[t][j].append( \
                    CompositeKeyFrame(Interpolation = "Boolean", \
                                      KeyTime = keytime, \
                                      KeyValues = [rowResult.GetArray(resultsLabelsDict[j][u]).GetTuple(ii)[0]]))
    DataRepresentation = Show()

for j in range(nResultFiles):
    for k in pos+rot:
        SMs_KF_Cue[k][j].KeyFrames = SMs_KF[k][j]
    DataRepresentation = Show()

if nResultFiles>1:
    TextId=[]
    for j in range(nResultFiles):
        TextId.append(Text())
        if resultLegend is None:
            TextId[-1].Text = op.splitext(op.basename(resultFiles[j]))[0]
        else:
            TextId[-1].Text = resultLegend[j]
        RenameSource("Filename_" + str(j), TextId[-1])
        DataRepresentation = Show()
        DataRepresentation.Color = defaultColor[j]
        DataRepresentation.Position = [0.15, 0.05+0.08*(nResultFiles-j)]
        DataRepresentation.FontSize = 12
        DataRepresentation.WindowLocation = 'AnyLocation'

timeLabel = AnnotateTimeFilter()
timeLabel_repr = Show()
timeLabel.Format = 'Time: %4.2f'
timeLabel_repr.FontSize = 14
timeLabel_repr.WindowLocation = 'UpperRightCorner'

# Hide initial transformation
for SM in SMs_tr_Initials:
    SetActiveSource(SM)
    renderView = GetActiveViewOrCreate('RenderView')
    Hide(SM, renderView)
    Hide3DWidgets(proxy = SM)

scene.Play()

Render()
