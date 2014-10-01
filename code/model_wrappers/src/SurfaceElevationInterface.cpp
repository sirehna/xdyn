/*
 * SurfaceElevationInterface.cpp
 *
 *  Created on: 22 avr. 2014
 *      Author: cady
 */

#include "SurfaceElevationInterface.hpp"
#include <ssc/kinematics.hpp>

template <typename PointType> PointType compute_relative_position(const PointType& P, const TR1(shared_ptr)<ssc::kinematics::Kinematics>& k)
{
    ssc::kinematics::Transform T = k->get("NED", P.get_frame());
    // Create the equivalent transformation just by swapping frame names
    T.swap();
    return T*P;
}

SurfaceElevationInterface::SurfaceElevationInterface(const TR1(shared_ptr)<ssc::kinematics::PointMatrix>& output_mesh_) : output_mesh(output_mesh_)
{
}

SurfaceElevationInterface::~SurfaceElevationInterface()
{
}

std::vector<ssc::kinematics::Point> SurfaceElevationInterface::get_points_on_free_surface(const double t, const TR1(shared_ptr)<ssc::kinematics::PointMatrix>& Mned) const
{
    std::vector<ssc::kinematics::Point> ret;
    for (int i = 0 ; i < output_mesh->m.cols() ; ++i)
    {
        // Ugly & dangerous hack: the points are in fact expressed in the NED frame
        // but we use output_mesh->get_frame() so that the observer can decide
        // whether to write the x & y coordinates at each time step (if the
        // output_mesh frame moves wrt NED) or once & for all at the beginning
        // of the YAML wave output (if output_mesh is expressed in the NED frame)
        const double x = Mned->m(0,i);
        const double y = Mned->m(1,i);
        const double z = wave_height(x,y,0,t);
        ret.push_back(ssc::kinematics::Point(output_mesh->get_frame(),x,y,z));
    }
    return ret;
}

double SurfaceElevationInterface::get_relative_wave_height(const ssc::kinematics::Point& P, const TR1(shared_ptr)<ssc::kinematics::Kinematics>& k, const double t) const
{
    const ssc::kinematics::Point OP = compute_relative_position(P, k);
    return wave_height(OP.x(),OP.y(),OP.z(),t);
}

std::vector<double> SurfaceElevationInterface::get_relative_wave_height(const ssc::kinematics::PointMatrix& P, const TR1(shared_ptr)<ssc::kinematics::Kinematics>& k, const double t) const
{
    const ssc::kinematics::PointMatrix OP = compute_relative_position(P, k);
    const int n = (int)P.m.cols();
    std::vector<double> ret;
    for (int i = 0 ; i < n ; ++i)
    {
        ret.push_back(wave_height((double)OP.m(0,i),(double)OP.m(1,i),(double)OP.m(2,i),t));
    }
    return ret;
}

TR1(shared_ptr)<ssc::kinematics::PointMatrix> SurfaceElevationInterface::get_output_mesh_in_NED_frame(const TR1(shared_ptr)<ssc::kinematics::Kinematics>& k) const
{
    if (output_mesh->get_frame() != "NED")
    {
        return TR1(shared_ptr)<ssc::kinematics::PointMatrix>(new ssc::kinematics::PointMatrix(k->get(output_mesh->get_frame(),"NED")*(*output_mesh)));
    }
    return output_mesh;
}

std::vector<ssc::kinematics::Point> SurfaceElevationInterface::get_waves_on_mesh(const TR1(shared_ptr)<ssc::kinematics::Kinematics>& k, //!< Object used to compute the transforms to the NED frame
                                              const double t //<! Current instant (in seconds)
                                             ) const
{
    if (output_mesh->m.cols()==0) return std::vector<ssc::kinematics::Point>();
    return get_points_on_free_surface(t, get_output_mesh_in_NED_frame(k));
}
