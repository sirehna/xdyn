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

ssc::kinematics::PointMatrix compute_relative_position(const Matrix3x& M,                                     //!< Points for which to compute the relative wave height
                                                       const std::string& frame,                              //!< Name of the reference frame in which the coordinates in M are expressed
                                                       const TR1(shared_ptr)<ssc::kinematics::Kinematics>& k //!< Object used to compute the transforms to the NED frame)
                                                       );

ssc::kinematics::PointMatrix compute_relative_position(const Matrix3x& M,                                     //!< Points for which to compute the relative wave height
                                                       const std::string& frame,                              //!< Name of the reference frame in which the coordinates in M are expressed
                                                       const TR1(shared_ptr)<ssc::kinematics::Kinematics>& k //!< Object used to compute the transforms to the NED frame)
                                                       )
{
    ssc::kinematics::Transform T = k->get("NED", frame);
    // Create the equivalent transformation just by swapping frame names
    T.swap();
    return T*ssc::kinematics::PointMatrix(M, frame);
}

template <typename PointType> PointType compute_relative_position(const TR1(shared_ptr)<PointType>& P, const TR1(shared_ptr)<ssc::kinematics::Kinematics>& k)
{
    ssc::kinematics::Transform T = k->get("NED", P->get_frame());
    // Create the equivalent transformation just by swapping frame names
    T.swap();
    return T*P;
}

SurfaceElevationInterface::SurfaceElevationInterface(const TR1(shared_ptr)<ssc::kinematics::PointMatrix>& output_mesh_) : output_mesh(output_mesh_),
        relative_wave_height_for_each_point_in_mesh(),
        surface_elevation_for_each_point_in_mesh()
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
        const double z = wave_height(x,y,t);
        ret.push_back(ssc::kinematics::Point(output_mesh->get_frame(),x,y,z));
    }
    return ret;
}

std::vector<double> SurfaceElevationInterface::get_relative_wave_height() const
{
    return relative_wave_height_for_each_point_in_mesh;
}

std::vector<double> SurfaceElevationInterface::get_surface_elevation() const
{
    return surface_elevation_for_each_point_in_mesh;
}

void SurfaceElevationInterface::update_surface_elevation(const ssc::kinematics::PointMatrixPtr& P,                     //!< Points for which to compute the relative wave height
                                                         const TR1(shared_ptr)<ssc::kinematics::Kinematics>& k, //!< Object used to compute the transforms to the NED frame
                                                         const double t //!< Current instant (in seconds)
       )
{
    const int n = (int)P->m.cols();
    if (n>0)
    {
        const ssc::kinematics::PointMatrix OP = compute_relative_position(P, k);
        relative_wave_height_for_each_point_in_mesh.resize((size_t)n);
        surface_elevation_for_each_point_in_mesh.resize((size_t)n);
        std::vector<double> ret;
        for (int i = 0 ; i < n ; ++i)
        {
            surface_elevation_for_each_point_in_mesh[(size_t)i] = wave_height((double)OP.m(0,i),(double)OP.m(1,i),t);
            relative_wave_height_for_each_point_in_mesh[(size_t)i] = (double)OP.m(2,i) - surface_elevation_for_each_point_in_mesh[(size_t)i];
        }
    }
}

double SurfaceElevationInterface::evaluate_rao(const double , //!< x-position of the RAO's calculation point in the NED frame (in meters)
                            const double , //!< y-position of the RAO's calculation point in the NED frame (in meters)
                            const double , //!< Current time instant (in seconds)
                            const std::vector<std::vector<std::vector<double> > >& , //!< Module of the RAO
                            const std::vector<std::vector<std::vector<double> > >&  //!< Phase of the RAO
                             ) const
{
    return 0;
}

std::vector<std::vector<double> > SurfaceElevationInterface::get_wave_directions_for_each_model() const
{
    return std::vector<std::vector<double> >();
}

std::vector<std::vector<double> > SurfaceElevationInterface::get_wave_angular_frequency_for_each_model() const
{
    return std::vector<std::vector<double> >();
}

double SurfaceElevationInterface::get_dynamic_pressure(const double rho, // Water density (in kg/m^3)
                                                       const double g, //!< Gravity (in m/s^2)
                                                       const ssc::kinematics::Point& P, //!< Position of point P, relative to the centre of the NED frame, but projected in any frame
                                                       const TR1(shared_ptr)<ssc::kinematics::Kinematics>& k, //!< Object used to compute the transforms to the NED frame
                                                       const double eta, //!< Wave elevation at P in the NED frame (in meters)
                                                       const double t //!< Current instant (in seconds)
                                    ) const
{
    const ssc::kinematics::Point OP = compute_relative_position(P, k);
    return dynamic_pressure(rho, g, OP.x(),OP.y(),OP.z(),eta,t);
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
                                              const double t //!< Current instant (in seconds)
                                             ) const
{
    if (output_mesh->m.cols()==0) return std::vector<ssc::kinematics::Point>();
    return get_points_on_free_surface(t, get_output_mesh_in_NED_frame(k));
}
