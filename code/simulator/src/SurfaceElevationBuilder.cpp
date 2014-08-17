/*
 * Res.cpp
 *
 *  Created on: Jul 1, 2014
 *      Author: cady
 */

#include "PointMatrix.hpp"
#include "SurfaceElevationBuilder.hpp"
#include "YamlWaveModelInput.hpp"

SurfaceElevationBuilderInterface::~SurfaceElevationBuilderInterface()
{
}

TR1(shared_ptr)<PointMatrix> SurfaceElevationBuilderInterface::make_wave_mesh(const YamlWaveOutput& output) const
{
    const size_t p = output.x.size();
    PointMatrix M(output.frame_of_reference, p);
    for (size_t j = 0 ; j < p ; ++j)
    {
        M.m(0,j) = output.x.at(j);
        M.m(1,j) = output.y.at(j);
        M.m(2,j) = 0;
    }
    return TR1(shared_ptr)<PointMatrix>(new PointMatrix(M));
}
