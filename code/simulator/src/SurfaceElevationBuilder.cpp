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
    const double dx = (output.xmax-output.xmin)/(output.nx > 1 ? output.nx-1 : 1);
    const double dy = (output.ymax-output.ymin)/(output.ny > 1 ? output.ny-1 : 1);
    PointMatrix M(output.frame_of_reference, output.nx*output.ny);
    for (size_t i = 0 ; i < output.nx ; ++i)
    {
        for (size_t j = 0 ; j < output.ny ; ++j)
        {
            M.m(0,j) = output.xmin + i*dx;
            M.m(1,j) = output.ymin + j*dy;
            M.m(2,j) = 0;
        }
    }
    return TR1(shared_ptr)<PointMatrix>(new PointMatrix(M));
}
