/*
 * SurfaceElevationInterface.cpp
 *
 *  Created on: 22 avr. 2014
 *      Author: cady
 */

#include "SurfaceElevationInterface.hpp"
#include "InternalErrorException.hpp"
#include <ssc/exception_handling.hpp>
#include <string>

/**
 * \note One can optimize the evaluation of the transformation in case only a
 * translation is required. For example, if transformation is defined from
 * NED to NED(body), only a translation is required
 */
template <typename PointType>
PointType compute_position_in_NED_frame(
        const PointType& P,
        const ssc::kinematics::KinematicsPtr& k)
{
    const std::string frame = P.get_frame();
    if (frame!="NED")
    {
        try
        {
            ssc::kinematics::Transform T = k->get("NED", P.get_frame());
            // Create the equivalent transformation just by swapping frame names
            T.swap();
            return T*P;
        }
        catch(const ssc::kinematics::KinematicsException& e)
        {
            THROW(__PRETTY_FUNCTION__, ssc::kinematics::KinematicsException, "frame '" << P.get_frame() << "' does not exist");
        }
    }
    return P;
}

SurfaceElevationInterface::SurfaceElevationInterface(
        const ssc::kinematics::PointMatrixPtr& output_mesh_,
        const std::pair<std::size_t,std::size_t>& output_mesh_size_) :
                output_mesh(output_mesh_),
                output_mesh_size(output_mesh_size_),
                relative_wave_height_for_each_point_in_mesh(),
                surface_elevation_for_each_point_in_mesh()
{
}

SurfaceElevationInterface::~SurfaceElevationInterface()
{
}

ssc::kinematics::PointMatrix SurfaceElevationInterface::get_points_on_free_surface(
        const double t,
        const ssc::kinematics::PointMatrixPtr& Mned
        ) const
{
    if (Mned->get_frame()!="NED")
    {
        std::stringstream ss;
        ss << "Points should be expressed in NED frame" <<std::endl;
        THROW(__PRETTY_FUNCTION__,ssc::exception_handling::Exception,ss.str());
    }
    ssc::kinematics::PointMatrix ret(*Mned);
    const size_t n = (size_t)Mned->m.cols();
    std::vector<double> x(n), y(n);
    for (size_t i = 0; i < n; ++i)
    {
        x[i] = (double)ret.m(0, i);
        y[i] = (double)ret.m(1, i);
    }
    const std::vector<double> wave_height_ = wave_height(x, y, t);
    for (size_t i = 0; i < n; ++i)
    {
        ret.m(2, i) = wave_height_.at(i);
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

void SurfaceElevationInterface::update_surface_elevation(
        const ssc::kinematics::PointMatrixPtr& P,       //!< Points for which to compute the relative wave height
        const ssc::kinematics::KinematicsPtr& k,        //!< Object used to compute the transforms to the NED frame
        const double t                                  //!< Current instant (in seconds)
        )
{
    const size_t n = (size_t)P->m.cols();
    if (n<=0) return;
    const ssc::kinematics::PointMatrix OP = compute_position_in_NED_frame(*P, k);
    relative_wave_height_for_each_point_in_mesh.resize(n);

    std::vector<double> x(n), y(n);
    for (size_t i = 0; i < n; ++i)
    {
        x[i] = (double)OP.m(0, i);
        y[i] = (double)OP.m(1, i);
    }
    surface_elevation_for_each_point_in_mesh = wave_height(x, y, t);
    for (size_t i = 0; i < n; ++i)
    {
        relative_wave_height_for_each_point_in_mesh[i] = (double)OP.m(2, i) - surface_elevation_for_each_point_in_mesh.at(i);
    }
}

double SurfaceElevationInterface::evaluate_rao(
        const double , //!< x-position of the RAO's calculation point in the NED frame (in meters)
        const double , //!< y-position of the RAO's calculation point in the NED frame (in meters)
        const double , //!< Current time instant (in seconds)
        const std::vector<std::vector<double> >& , //!< Module of the RAO
        const std::vector<std::vector<double> >&  //!< Phase of the RAO
        ) const
{
    return 0;
}

ssc::kinematics::PointMatrix SurfaceElevationInterface::orbital_velocity(
    const double,                  //!< gravity (in m/s^2)
    const std::vector<double>& x,  //!< x-positions in the NED frame (in meters)
    const std::vector<double>& y,  //!< y-positions in the NED frame (in meters)
    const std::vector<double>& z,  //!< z-positions in the NED frame (in meters)
    const double,                  //!< current instant (in seconds)
    const std::vector<double>& eta //!< Wave elevations at (x,y) in the NED frame (in meters)
    ) const
{
    if (x.size() != y.size() || x.size() != z.size() || x.size() != eta.size())
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Error when calculating orbital velocity: the x, y, z and eta vectors don't have the same size (size of x: "
            << x.size() << ", size of y: " << y.size() << ", size of z: " << z.size() << ", size of eta: " << eta.size() << ")");
    }
    return ssc::kinematics::PointMatrix(ssc::kinematics::Matrix3Xd::Zero(3,x.size()), "NED");
}

std::vector<std::vector<double> > SurfaceElevationInterface::get_wave_directions_for_each_model() const
{
    return std::vector<std::vector<double> >();
}

std::vector<std::vector<double> > SurfaceElevationInterface::get_wave_angular_frequency_for_each_model() const
{
    return std::vector<std::vector<double> >();
}

std::vector<double> SurfaceElevationInterface::get_dynamic_pressure(
    const double rho,                        //!< Water density (in kg/m^3)
    const double g,                          //!< Gravity (in m/s^2)
    const ssc::kinematics::PointMatrix& P,   //!< Positions of points P, relative to the centre of the NED frame, but projected in any frame
    const ssc::kinematics::KinematicsPtr& k, //!< Object used to compute the transforms to the NED frame
    const std::vector<double>& eta,          //!< Wave elevations at P in the NED frame (in meters)
    const double t                           //!< Current instant (in seconds)
    ) const
{
    const size_t n = (size_t)P.m.cols();
    if (n != eta.size())
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException,
            "Error when calculating dynamic pressure: the vector of positions of points P and the vector of their corresponding wave elevations don't have the same size (size of P: "
                << n << ", size of eta: " << eta.size() << ")")
    }
    const ssc::kinematics::PointMatrix OP = compute_position_in_NED_frame(P, k);
    std::vector<double> x(n), y(n), z(n);
    for (size_t i = 0; i < n; ++i) {;
        x.at(i) = OP.m(0, i);
        y.at(i) = OP.m(1, i);
        z.at(i) = OP.m(2, i);
    }
    return dynamic_pressure(rho, g, x, y, z, eta, t);
}

ssc::kinematics::PointMatrixPtr SurfaceElevationInterface::get_output_mesh_in_NED_frame(
        const ssc::kinematics::KinematicsPtr& k) const
{
    using namespace ssc::kinematics;
    return PointMatrixPtr(new PointMatrix(compute_position_in_NED_frame(*output_mesh,k)));
}

ssc::kinematics::PointMatrix SurfaceElevationInterface::get_waves_on_mesh(
        const ssc::kinematics::KinematicsPtr& k, //!< Object used to compute the transforms to the NED frame
        const double t                           //!< Current instant (in seconds)
        ) const
{
    if (output_mesh->m.cols()==0) return ssc::kinematics::PointMatrix("NED",0);
    return get_points_on_free_surface(t, get_output_mesh_in_NED_frame(k));
}

SurfaceElevationGrid SurfaceElevationInterface::get_waves_on_mesh_as_a_grid(
        const ssc::kinematics::KinematicsPtr& k,    //!< Object used to compute the transforms to the NED frame
        const double t                              //!< Current instant (in seconds)
        ) const
{
    ssc::kinematics::PointMatrix res = get_waves_on_mesh(k,t);
    const size_t nPoints = (size_t)res.m.cols();
    if (nPoints==0) return SurfaceElevationGrid(t);
    const size_t nx = output_mesh_size.first;
    const size_t ny = output_mesh_size.second;
    if ((nx*ny)!=nPoints)
    {
        std::stringstream ss;
        ss << "SurfaceElevation Problem : " <<std::endl
           << "nx*ny = "<<nx << "*"<<ny <<"="<<nx*ny
           <<" should be equal to res.size() = "<<nPoints<<std::endl<<std::endl
           <<" Make sure your 'environment models' are correctly defined for waves"<<std::endl
           <<" For example, if one declares a 'no waves' model, one should not have an 'output' section"<<std::endl;
        THROW(__PRETTY_FUNCTION__, ssc::exception_handling::Exception,ss.str());
    }
    SurfaceElevationGrid s(nx,ny,t);
    for(long i=0;i<(long)nx;++i)
    {
        s.x(i) = res.m(0,i);
    }
    for(long j=0; j<(long)ny; ++j)
    {
        s.y(j) = res.m(1,j*((long)nx));
    }
    long idx = 0;
    for(long j=0;j<(long)ny;++j)
    {
        for(long i = 0;i<(long)nx;++i)
        {
            s.z(i,j) = res.m(2,idx++);
        }
    }
    return s;
}

void SurfaceElevationInterface::serialize_wave_spectra_before_simulation(ObserverPtr&) const
{
}
