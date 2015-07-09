/*
 * SurfaceElevationBuilder.cpp
 *
 *  Created on: Jul 1, 2014
 *      Author: cady
 */

#include "SurfaceElevationBuilder.hpp"
#include "YamlWaveModelInput.hpp"

SurfaceElevationBuilderInterface::~SurfaceElevationBuilderInterface()
{
}

std::pair<double,double> get_wave_mesh_step_size(const YamlWaveOutput& output);
std::pair<double,double> get_wave_mesh_step_size(const YamlWaveOutput& output)
{
    const double dx = (output.xmax-output.xmin)/(double)(output.nx > 1 ? output.nx-1 : 1);
    const double dy = (output.ymax-output.ymin)/(double)(output.ny > 1 ? output.ny-1 : 1);
    return std::make_pair(dx,dy);
}

std::pair<std::size_t,std::size_t> SurfaceElevationBuilderInterface::get_wave_mesh_size(const YamlWaveOutput& output) const
{
    const std::pair<double,double> dxdy = get_wave_mesh_step_size(output);
    const size_t nx = (dxdy.first==0 and output.nx) ? 1 : output.nx;
    const size_t ny = (dxdy.second==0 and output.ny) ? 1 : output.ny;
    return std::make_pair(nx,ny);
}

ssc::kinematics::PointMatrixPtr SurfaceElevationBuilderInterface::make_wave_mesh(const YamlWaveOutput& output)
{
    using namespace ssc::kinematics;
    const std::pair<double,double> dxdy = get_wave_mesh_step_size(output);
    const double dx = dxdy.first;
    const double dy = dxdy.second;
    const size_t nx = (dx==0 and output.nx) ? 1 : output.nx;
    const size_t ny = (dy==0 and output.ny) ? 1 : output.ny;
    PointMatrixPtr M = PointMatrixPtr(new PointMatrix(output.frame_of_reference, nx*ny));
    int k = 0;
    for (size_t j = 0 ; j < ny ; ++j)
    {
        const double y = output.ymin + (double)j*dy;
        for (size_t i = 0 ; i < nx ; ++i)
        {
            M->m(0,k) = output.xmin + (double)i*dx;
            M->m(1,k) = y;
            M->m(2,k) = 0;
            ++k;
        }
    }
    return M;
}
