/*
 * DefaultWaveModel.cpp
 *
 *  Created on: 22 avr. 2014
 *      Author: cady
 */

#include "DefaultWaveModel.hpp"

DefaultWaveModel::DefaultWaveModel(const double wave_height_) : wave_height(wave_height_)
{
}

double DefaultWaveModel::get_relative_wave_height(const Point&) const
{
    return 0;
}
