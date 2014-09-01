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
    size_t k = 0;
    for (size_t j = 0 ; j < output.ny ; ++j)
    {
        for (size_t i = 0 ; i < output.nx ; ++i)
        {
            M.m(0,k) = output.xmin + i*dx;
            M.m(1,k) = output.ymin + j*dy;
            M.m(2,k) = 0;
            ++k;
        }
    }
    return TR1(shared_ptr)<PointMatrix>(new PointMatrix(M));
}
