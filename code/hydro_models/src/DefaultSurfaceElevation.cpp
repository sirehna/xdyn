/*
 * DefaultSurfaceElevation.cpp
 *
 *  Created on: 22 avr. 2014
 *      Author: cady
 */

#include "DefaultSurfaceElevation.hpp"

DefaultSurfaceElevation::DefaultSurfaceElevation(const double wave_height_, const TR1(shared_ptr)<PointMatrix>& output_mesh_) : WaveModelInterface(output_mesh_), zwave(wave_height_)
{
}

double DefaultSurfaceElevation::wave_height(const double , const double , const double z, const double ) const
{
    return z - zwave ;
}
