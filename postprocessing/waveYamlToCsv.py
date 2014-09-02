# -*- encoding: utf-8 -*-
#
# Script used to convert YAML output wave files to CSV, that are used
# by Paraview to represent wave elevation.
#
# Valid input YAML files can be
#
# waves:
#   x : [-1,1,1,-1]
#   y : [-1,-1,1,1]
#   timesteps:
#     - t: 0
#       z : [1,1,1,1]
#     - t: 1
#       z : [1,1,1,1]
#
# waves:
#   timesteps:
#     - t: 0
#       x : [-1,1,1,-1]
#       y : [-1,-1,1,1]
#       z : [1,1,1,1]
#     - t: 1
#       x : [-1,1,1,-1]
#       y : [-1,-1,1,1]
#       z : [1,1,1,1]

import argparse
import os
import re
import numpy as np

def generateCsvWaveFilesFromYamlOutputWaveFile(
        yamlWave = 'wave.yml',
        CsvFormat = '%.18e',
        useYamlParser = False):
    def generateCsvWaveFilesFromYamlOutputWaveFileWithYamlParser():
        import yaml
        waves = yaml.load(open(yamlWave, 'r'))
        waves = waves['waves']
        has_x_and_y_field_for_each_t = True
        if waves.has_key('x') and waves.has_key('y'):
            has_x_and_y_field_for_each_t = False
            x = waves['x']
            y = waves['y']
        timesteps = waves['timesteps']
        for i, timestep in enumerate(timesteps):
            if has_x_and_y_field_for_each_t:
                x = timestep['x']
                y = timestep['y']
            z = timestep['z']
            XYZ = np.concatenate([np.vstack(x), np.vstack(y), np.vstack(z)], axis = 1)
            np.savetxt(os.path.join(outputDirectory, 'waves_{0:05d}.csv'.format(i)), \
                       XYZ, fmt = CsvFormat, delimiter = ',')
    def generateCsvWaveFilesFromYamlOutputWaveFileManually():
        res = open(yamlWave, 'r').readlines()
        if re.search("^\s+x", res[1]):
            x = res[1]
            y = res[2]
            X = np.vstack(map(float, x.split('[')[1].split(']')[0].split(',')))
            Y = np.vstack(map(float, y.split('[')[1].split(']')[0].split(',')))
            for k, i in enumerate(range(5, len(res), 2)):
                z = res[i]
                Z = np.vstack(map(float, z.split('[')[1].split(']')[0].split(',')))
                XYZ = np.concatenate([X, Y, Z], axis = 1)
                np.savetxt(os.path.join(outputDirectory, \
                                        'waves_{0:05d}.csv'.format(k)), \
                           XYZ, fmt = CsvFormat, delimiter = ',')
        else:
            for k, i in enumerate(range(3, len(res), 4)):
                x = res[i]
                y = res[i + 1]
                z = res[i + 2]
                X = np.vstack(map(float, x.split('[')[1].split(']')[0].split(',')))
                Y = np.vstack(map(float, y.split('[')[1].split(']')[0].split(',')))
                Z = np.vstack(map(float, z.split('[')[1].split(']')[0].split(',')))
                XYZ = np.concatenate([X, Y, Z], axis = 1)
                np.savetxt(os.path.join(outputDirectory, 'waves_{0:05d}.csv'.format(k)), \
                           XYZ, fmt = CsvFormat, delimiter = ',')
    outputDirectory = os.path.splitext(yamlWave)[0]
    if not os.path.isdir(outputDirectory):
        os.mkdir(outputDirectory)
    if useYamlParser:
        generateCsvWaveFilesFromYamlOutputWaveFileWithYamlParser()
    else:
        # Manual parsing of YAML files, to avoid memory explosion when parsing
        # YAML file with
        generateCsvWaveFilesFromYamlOutputWaveFileManually()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description = 'Convert YAML output wave files to CSV')
    parser.add_argument("-y", "--yaml",
                        help = "Name of the YAML files to process",
                        nargs = '*',
                        default = r'wave.yaml')
    args = parser.parse_args()
    for y in args.yaml:
        generateCsvWaveFilesFromYamlOutputWaveFile(yamlWave = y)
