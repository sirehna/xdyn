/*
 * DefaultWaveModel.cpp
 *
 *  Created on: 22 avr. 2014
 *      Author: cady
 */

#include "DefaultWaveModel.hpp"

DefaultWaveModel::DefaultWaveModel(const double wave_height_) : WaveModelInterface(), zwave(wave_height_)
{
}

double DefaultWaveModel::wave_height(double , double , double z) const
{
    return z - zwave ;
}
