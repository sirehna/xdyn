# -*- coding: utf-8 -*-
#
import argparse
import sys
import pandas as pd

def _addEulerAnglesToDataframe(R, output = '', toDegree = False):
    def getQuaternion(R, output = ''):
        q = R[['qr(' + output + ')', 'qi(' + output + ')', 'qj(' + output + ')', 'qk(' + output + ')']]
        q.columns = ['qr', 'qi', 'qj', 'qk']
        q.name = output
        return q
    def quat2eul(q, convention = 321, toDegree = False):
        from numpy import arctan2, arcsin, pi
        if convention == 321:
            phi = arctan2(2 * (q['qr'] * q['qi'] + q['qj'] * q['qk']), 1 - 2 * (q['qi'] ** 2 + q['qj'] ** 2))
            theta = arcsin(2 * (q['qr'] * q['qj'] - q['qk'] * q['qi']))
            psi = arctan2(2 * (q['qr'] * q['qk'] + q['qi'] * q['qj']), 1 - 2 * (q['qj'] ** 2 + q['qk'] ** 2))
            phi_theta_psi = pd.concat([phi, theta, psi], axis = 1)
            phi_theta_psi.columns = ['phi(' + q.name + ')', 'theta(' + q.name + ')', 'psi(' + q.name + ')']
        else:
            raise Exception
        if toDegree:
            phi_theta_psi *= 180.0 / pi
        return phi_theta_psi
    q = getQuaternion(R, output)
    R = pd.concat([R, quat2eul(q, convention = 321, toDegree = True)], axis = 1)
    return R

def addEulerAnglesToDataframe(input = '', output = '', names = '', toDegree = True):
    if input == '':
        R = pd.read_csv(sys.stdin)
    else:
        R = pd.read_csv(input)
    if isinstance(names, (str, unicode)):
        names = [names]
    for n in names:
        R = _addEulerAnglesToDataframe(R, output = n, toDegree = toDegree)
    if output == '':
        R.to_csv(sys.stdout, index = False)
    else:
        R.to_csv(output, index = False)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description = '')
    parser.add_argument("-i", "--input",
                        help = "Name of the file to process",
                        nargs = '*',
                        default = '')
    parser.add_argument("-n", "--name",
                        help = "Name of the bodies to process",
                        nargs = '*',
                        default = [r'cube'])
    parser.add_argument("-o", "--output",
                        help = "Name of the file to process",
                        nargs = '*',
                        default = '')
    args = parser.parse_args()
    addEulerAnglesToDataframe(input = args.input, output = args.output, names = args.name)
