/*
 * Res.cpp
 *
 *  Created on: Jul 1, 2014
 *      Author: cady
 */

#include <ssc/kinematics.hpp>
#include "SurfaceElevationBuilder.hpp"
#include "YamlWaveModelInput.hpp"

SurfaceElevationBuilderInterface::~SurfaceElevationBuilderInterface()
{
}

TR1(shared_ptr)<ssc::kinematics::PointMatrix> SurfaceElevationBuilderInterface::make_wave_mesh(const YamlWaveOutput& output) const
{
    const double dx = (output.xmax-output.xmin)/(double)(output.nx > 1 ? output.nx-1 : 1);
    const double dy = (output.ymax-output.ymin)/(double)(output.ny > 1 ? output.ny-1 : 1);
    const size_t nx = (dx==0 and output.nx) ? 1 : output.nx;
    const size_t ny = (dy==0 and output.ny) ? 1 : output.ny;
    ssc::kinematics::PointMatrix M(output.frame_of_reference, nx*ny);
    int k = 0;
    for (int j = 0 ; j < (int)ny ; ++j)
    {
        for (int i = 0 ; i < (int)nx ; ++i)
        {
            M.m(0,k) = output.xmin + (double)i*dx;
            M.m(1,k) = output.ymin + (double)j*dy;
            M.m(2,k) = 0;
            ++k;
        }
    }
    return TR1(shared_ptr)<ssc::kinematics::PointMatrix>(new ssc::kinematics::PointMatrix(M));
}
