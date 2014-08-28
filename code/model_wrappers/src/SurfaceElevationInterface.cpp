/*
 * SurfaceElevationInterface.cpp
 *
 *  Created on: 22 avr. 2014
 *      Author: cady
 */

#include "SurfaceElevationInterface.hpp"
#include "Kinematics.hpp"
#include "Point.hpp"
#include "Transform.hpp"
#include "PointMatrix.hpp"

template <typename PointType> PointType compute_relative_position(const PointType& P, const TR1(shared_ptr)<Kinematics>& k)
{
    kinematics::Transform T = k->get("NED", P.get_frame());
    // Create the equivalent transformation just by swapping frame names
    T.swap();
    return T*P;
}

SurfaceElevationInterface::SurfaceElevationInterface(const TR1(shared_ptr)<PointMatrix>& output_mesh_) : output_mesh(output_mesh_)
{
}

SurfaceElevationInterface::~SurfaceElevationInterface()
{
}

double SurfaceElevationInterface::get_relative_wave_height(const Point& P, const TR1(shared_ptr)<Kinematics>& k, const double t) const
{
    const Point OP = compute_relative_position(P, k);
    return wave_height(OP.x(),OP.y(),OP.z(),t);
}

std::vector<double> SurfaceElevationInterface::get_relative_wave_height(const PointMatrix& P, const TR1(shared_ptr)<Kinematics>& k, const double t) const
{
    const PointMatrix OP = compute_relative_position(P, k);
    const int n = (int)P.m.cols();
    std::vector<double> ret;
    for (int i = 0 ; i < n ; ++i)
    {
        ret.push_back(wave_height((double)OP.m(0,i),(double)OP.m(1,i),(double)OP.m(2,i),t));
    }
    return ret;
}

std::vector<EPoint> SurfaceElevationInterface::get_waves_on_mesh(const TR1(shared_ptr)<Kinematics>& k, //!< Object used to compute the transforms to the NED frame
                                              const double  //<! Current instant (in seconds)
                                             ) const
{
    k->get(output_mesh->get_frame(), "NED");
    return std::vector<EPoint>(output_mesh->m.cols(), EPoint());
}
