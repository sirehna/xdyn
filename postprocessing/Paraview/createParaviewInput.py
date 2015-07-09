import os
from sys import version_info
import numpy as np
import h5py
from math import hypot, atan2, pi
from math import cos, sin

from pint import UnitRegistry
ureg = UnitRegistry()

if version_info[0] > 2:
    openFile = open
else:
    import codecs
    import warnings
    def openFile(filename, mode = 'r', buffering = -1, encoding = None,
             errors = None, newline = None, closefd = True, opener = None):
        if newline is not None:
            warnings.warn('newline is not supported in py2')
        if not closefd:
            warnings.warn('closefd is not supported in py2')
        if opener is not None:
            warnings.warn('opener is not supported in py2')
        return codecs.open(filename = filename, mode = mode, encoding = encoding,
                    errors = errors, buffering = buffering)

def convertToSI(s = 'm', dictException = {}):
    if isinstance(s, (list, tuple, set)):
        return [convertToSI(s = i, dictException = dictException) for i in s]
    ss = s.strip()
    if ss in dictException.keys():
        return dictException[ss]
    else:
        return ureg[ss].to_base_units().magnitude

def dictUnitValueToSi(d = {'unit': 'm', 'value': 0.0}):
    hasUnit = 'unit' in d
    hasValue = 'value' in d
    if hasUnit and hasValue:
        return convertToSI(d['unit']) * d['value']
    elif hasValue:
        return d['value']
    else:
        raise Exception('')

def eul321ToCtm(phi = 0.0, theta = 0.0, psi = 0.0):
    cosPhi = cos(phi)
    cosTheta = cos(theta)
    cosPsi = cos(psi)
    sinPhi = sin(phi)
    sinTheta = sin(theta)
    sinPsi = sin(psi)
    ctm = np.zeros((3, 3))
    ctm[0, 0] = cosPsi * cosTheta
    ctm[0, 1] = sinPsi * cosTheta
    ctm[0, 2] = -sinTheta
    ctm[1, 0] = sinPhi * cosPsi * sinTheta - cosPhi * sinPsi
    ctm[1, 1] = sinPhi * sinPsi * sinTheta + cosPhi * cosPsi
    ctm[1, 2] = sinPhi * cosTheta
    ctm[2, 0] = cosPhi * cosPsi * sinTheta + sinPhi * sinPsi
    ctm[2, 1] = cosPhi * sinPsi * sinTheta - sinPhi * cosPsi
    ctm[2, 2] = cosPhi * cosTheta
    return ctm

def eul321ToRot(phi = 0.0, theta = 0.0, psi = 0.0):
    return eul321ToCtm(phi, theta, psi).transpose()

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

def ctm2eul312(ctm, useDegree = False, eps = 1e-13):
    hyp = hypot(ctm[1, 0], ctm[1, 1]) # cos(phi)
    phi = atan2(ctm[1, 2], hyp)
    if(hyp > eps):
        # not degenerated
        theta = atan2(-ctm[0, 2], ctm[2, 2])
        psi = atan2(-ctm[1, 0], ctm[1, 1])
    else:
        theta = 0
        psi = atan2(ctm[0, 1], ctm[0, 0])
    euler = [phi, theta, psi]
    if useDegree:
        euler = [k * 180.0 / pi for k in euler]
    return euler

def generateVtkTrajectoryFileFromH5(state, exportFilename = 'state.vtk'):
    X, Y, Z, Quat = state['X'], state['Y'], state['Z'], state['Quat']
    Qr, Qi, Qj, Qk = Quat['Qr'], Quat['Qi'], Quat['Qj'], Quat['Qk']
    nombreDePasDeSimulation = len(X)
    f = open(exportFilename, 'w')
    f.write("# vtk DataFile Version 3.0\n")
    f.write("Export des reperes pour une simulation aux differents pas de temps - SIREHNA\n")
    f.write("ASCII\n")
    f.write("DATASET POLYDATA\n")
    f.write("POINTS {0} float\n".format(3 * nombreDePasDeSimulation))
    for i in range(nombreDePasDeSimulation):
        strToWrite = " {0:.5f} {1:.5f} {2:.5f}\n".format(X[i], Y[i], Z[i])
        f.write(strToWrite)
        f.write(strToWrite)
        f.write(strToWrite)
    f.write("\n")
    f.write("VERTICES 3 {0}\n".format(6 * nombreDePasDeSimulation))
    for i in range(3 * nombreDePasDeSimulation):
        f.write("1 {0}\n".format(i))
    f.write("\n")
    f.write("POINT_DATA {0}\n".format(3 * nombreDePasDeSimulation))
    f.write("VECTORS triedre float\n")
    for i in range(nombreDePasDeSimulation):
        ctm = quat2ctm([Qr[i], Qi[i], Qj[i], Qk[i]])
        f.write(" {0:.5f} {1:.5f} {2:.5f}\n".format(ctm[0, 0], ctm[0, 1], ctm[0, 2]))
        f.write(" {0:.5f} {1:.5f} {2:.5f}\n".format(ctm[1, 0], ctm[1, 1], ctm[1, 2]))
        f.write(" {0:.5f} {1:.5f} {2:.5f}\n".format(ctm[2, 0], ctm[2, 1], ctm[2, 2]))
    f.close()

def generateCsvTrajectoryFileFromH5(state, t = None, exportFilename = 'state.csv'):
    X = state['X']
    Y = state['Y']
    Z = state['Z']
    Quat = state['Quat']
    Qr, Qi, Qj, Qk = Quat['Qr'], Quat['Qi'], Quat['Qj'], Quat['Qk']
    nombreDePasDeSimulation = len(X)
    if t is None:
        t = np.arange(nombreDePasDeSimulation)
    f = open(exportFilename, 'w')
    f.write("Time,X,Y,Z,ParaviewRotDegX,ParaviewRotDegY,ParaviewRotDegZ\n")
    for i in range(nombreDePasDeSimulation):
        euler = ctm2eul312(quat2ctm([Qr[i], Qi[i], Qj[i], Qk[i]]), useDegree = True)
        strToWrite = "{0},{1:.5f},{2:.5f},{3:.5f},{4:.5f},{5:.5f},{6:.5f}\n".\
                    format(t[i], X[i], Y[i], Z[i], euler[0], euler[1], euler[2])
        f.write(strToWrite)
    f.close()

def doesStateHasAllRequiredKeys(state, requiredStateKeys = ('X', 'Y', 'Z', 'Quat')):
    for k in requiredStateKeys:
        if k not in state.keys():
            return False
    return True

def generateParaviewScript(yamlInputFile, h5Filename, pvPythonTemplate = "MacroParaviewAnimate.py", outputFile = None):
    import jinja2
    if not os.path.isabs(pvPythonTemplate):
        pvPythonTemplate = os.path.join(os.getcwd(), pvPythonTemplate)
    if not os.path.exists(pvPythonTemplate):
        raise IOError("File '{0}' not found".foramt(pvPythonTemplate))
    dirname, basename = os.path.dirname(pvPythonTemplate), os.path.basename(pvPythonTemplate)
    templateLoader = jinja2.FileSystemLoader(searchpath = dirname)
    templateEnv = jinja2.Environment(loader = templateLoader)
    template = templateEnv.get_template(basename)
    p = ParaviewYaml(yamlInputFile)
    dictStlFiles = p.getStlNames()
    dictOffsets = p.getOffsets()
    SM_path, resultFiles, resultLegend = [], [], []
    for animatedBody in getListOfAnimatedBodies(h5Filename):
        if animatedBody in dictStlFiles:
            SM_path.append(generateStlFileName(h5Filename, dictStlFiles[animatedBody]))
            resultFiles.append(generateCsvResultFileName(h5Filename, animatedBody))
            resultLegend.append(animatedBody)
    templateVars = {
        'waveDirectory'         : os.path.splitext(h5Filename)[0] + '_Waves',
        'offsets'               : repr(dictOffsets),
        'SM_path'               : SM_path,
        'simulationDirectory'   : os.getcwd(),
        'resultFiles'           : resultFiles,
        'resultLegend'          : resultLegend}
    outputText = template.render(templateVars)
    if outputFile is None:
        print(outputText)
    else:
        with openFile(outputFile, mode = 'w', encoding = 'utf-8') as stream:
            stream.write(outputText)

class ParaviewYaml():
    def __init__(self, yamlInputFile = 'case3_sinus_Fremm.yml'):
        import yaml
        stream = open(yamlInputFile, 'r').read()
        self.y = yaml.load(stream)
    def getStlNames(self):
        return {body['name']:body['mesh'] for body in self.y['bodies']}
    def getOffsets(self):
        convention = self.y['rotations convention']
        if convention != ["psi", "theta'", "phi''"]:
            raise Exception('Euler convention not implemented')
        convention = 321
        offsets = {}
        for body in self.y['bodies']:
            p = body['position of body frame relative to mesh']
            if p['frame'] != 'mesh':
                raise Exception('Euler convention not implemented')
            offset = {'X':-dictUnitValueToSi(p['x']), \
                      'Y':-dictUnitValueToSi(p['y']), \
                      'Z':-dictUnitValueToSi(p['z'])}
            eul = [dictUnitValueToSi(p[k]) for k in ('phi', 'theta', 'psi')]
            prot = ctm2eul312(eul321ToRot(*eul), useDegree = True)
            offset.update({'RotX':prot[0], 'RotY':prot[1], 'RotZ':prot[2]})
            offsets[body['name']] = offset
        return offsets

def getListOfAnimatedBodies(h5Filename):
    listOfAnimatedBodies = []
    h = h5py.File(h5Filename, 'r')
    outputs = h['outputs']
    states = outputs['states']
    keys = states.keys()
    requiredStateKeys = ('X', 'Y', 'Z', 'Quat')
    for k in keys:
        state = states[k]
        if doesStateHasAllRequiredKeys(state, requiredStateKeys):
            listOfAnimatedBodies.append(k)
    h.close()
    return listOfAnimatedBodies

def generateStlFileName(h5Filename, k):
    if not os.path.isabs(h5Filename):
        h5Filename = os.path.join(os.getcwd(), h5Filename)
    return os.path.join(os.path.dirname(h5Filename), k)
def generateVtkResultFileName(h5Filename, k):
    return os.path.splitext(h5Filename)[0] + '_Traj_' + k + '.vtk'
def generateCsvResultFileName(h5Filename, k):
    return os.path.splitext(h5Filename)[0] + '_Traj_' + k + '.csv'

def createParaviewTrajInputs(h5Filename, verbose = False):
    h = h5py.File(h5Filename, 'r')
    outputs = h['outputs']
    t = outputs['t'] if 't' in outputs else None
    states = outputs['states']
    keys = states.keys()
    requiredStateKeys = ('X', 'Y', 'Z', 'Quat')
    for k in keys:
        if verbose:
            print("Deal object {0}".format(k))
        state = states[k]
        if doesStateHasAllRequiredKeys(state, requiredStateKeys):
            vtkTrajName = generateVtkResultFileName(h5Filename, k)
            if verbose:
                print("Created file {0}".format(vtkTrajName))
            generateVtkTrajectoryFileFromH5(state, vtkTrajName)
            csvTrajName = generateCsvResultFileName(h5Filename, k)
            generateCsvTrajectoryFileFromH5(state, t, csvTrajName)
            if verbose:
                print("Created file {0}".format(csvTrajName))
    h.close()

def main(h5Filename, yamlInputFile = '', paraviewTemplateFile = '', verbose = False):
    createParaviewTrajInputs(h5Filename, verbose)
    conditionNotToActivateScriptGeneration = yamlInputFile == '' or paraviewTemplateFile == ''
    if conditionNotToActivateScriptGeneration:
        return
    pScript = os.path.splitext(paraviewTemplateFile)[0] + '_' + os.path.splitext(os.path.basename(h5Filename))[0] + '.py'
    if verbose:
        print("Create Paraview macro {0}".format(pScript))
    generateParaviewScript(yamlInputFile, h5Filename, pvPythonTemplate = paraviewTemplateFile, outputFile = pScript)

if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description = 'Generate ASCII files for Paraview visualization, and also optionnaly a Paraview animation script')
    parser.add_argument("--hdf5",
                        help = "Name of the HDF5 output file",
                        default = '')
    parser.add_argument("-y", "--yaml",
                        default = "",
                        help = "Option used to generate")
    parser.add_argument("-t", "--templateParaviewFile",
                        default = "MacroParaviewAnimate.py",
                        help = "Option used to generate")
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
    main(args.hdf5, args.yaml, args.templateParaviewFile, verbose = args.verbose)
