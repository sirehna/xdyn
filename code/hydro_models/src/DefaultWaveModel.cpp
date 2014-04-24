/*
 * DefaultWaveModel.cpp
 *
 *  Created on: 22 avr. 2014
 *      Author: cady
 */

#include "DefaultWaveModel.hpp"

DefaultWaveModel::DefaultWaveModel(const double wave_height_, const std::tr1::shared_ptr<Kinematics>& k_) : WaveModelInterface(k_), zwave(wave_height_)
{
}

double DefaultWaveModel::wave_height(double , double , double z) const
{
    return zwave - z;
}
