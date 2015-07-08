/*
 * SurfaceElevationInterface.cpp
 *
 *  Created on: 22 avr. 2014
 *      Author: cady
 */

#include "SurfaceElevationInterface.hpp"
#include <ssc/exception_handling.hpp>

template <typename PointType> PointType compute_relative_position(
        const PointType& P,
        const ssc::kinematics::KinematicsPtr& k)
{
    ssc::kinematics::Transform T = k->get("NED", P.get_frame());
    // Create the equivalent transformation just by swapping frame names
    T.swap();
    return T*P;
}

ssc::kinematics::PointMatrix compute_relative_position(
        const Matrix3x& M,                                      //!< Points for which to compute the relative wave height
        const std::string& frame,                               //!< Name of the reference frame in which the coordinates in M are expressed
        const ssc::kinematics::KinematicsPtr& k                 //!< Object used to compute the transforms to the NED frame)
        );

ssc::kinematics::PointMatrix compute_relative_position(
        const Matrix3x& M,                                     //!< Points for which to compute the relative wave height
        const std::string& frame,                              //!< Name of the reference frame in which the coordinates in M are expressed
        const ssc::kinematics::KinematicsPtr& k                //!< Object used to compute the transforms to the NED frame)
        )
{
    ssc::kinematics::Transform T = k->get("NED", frame);
    // Create the equivalent transformation just by swapping frame names
    T.swap();
    return T*ssc::kinematics::PointMatrix(M, frame);
}

#define CHECK(x) if (std::isnan(x)) {THROW(__PRETTY_FUNCTION__,ssc::exception_handling::Exception,"NaN detected in " QUOTEME(x));}

template <typename PointType> PointType compute_relative_position(const TR1(shared_ptr)<PointType>& P, const TR1(shared_ptr)<ssc::kinematics::Kinematics>& k)
{
    ssc::kinematics::Transform T = k->get("NED", P->get_frame());
    CHECK(T.get_point().v(0));
    CHECK(T.get_point().v(1));
    CHECK(T.get_point().v(2));
    CHECK(T.get_rot()(0,0));
    CHECK(T.get_rot()(0,1));
    CHECK(T.get_rot()(0,2));
    CHECK(T.get_rot()(1,0));
    CHECK(T.get_rot()(1,1));
    CHECK(T.get_rot()(1,2));
    CHECK(T.get_rot()(2,0));
    CHECK(T.get_rot()(2,1));
    CHECK(T.get_rot()(2,2));
    // Create the equivalent transformation just by swapping frame names
    T.swap();
    return T*P;
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

std::vector<ssc::kinematics::Point> SurfaceElevationInterface::get_points_on_free_surface(
        const double t,
        const ssc::kinematics::PointMatrixPtr& Mned
        ) const
{
    if (Mned->get_frame()!="NED")
    {
        std::stringstream ss;
        ss << "Problem : " <<std::endl;
        THROW(__PRETTY_FUNCTION__,ssc::exception_handling::Exception,ss.str());
    }
    std::vector<ssc::kinematics::Point> ret(output_mesh->m.cols());
    for (int i = 0 ; i < output_mesh->m.cols() ; ++i)
    {
        const double x = Mned->m(0,i);
        const double y = Mned->m(1,i);
        const double z = wave_height(x,y,t);
        ret[i] = ssc::kinematics::Point("NED",x,y,z);
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
    const int n = (int)P->m.cols();
    if (n<=0) return;
    const ssc::kinematics::PointMatrix OP = compute_position_in_NED_frame(*P, k);
    relative_wave_height_for_each_point_in_mesh.resize((size_t)n);
    surface_elevation_for_each_point_in_mesh.resize((size_t)n);
    for (int i = 0 ; i < n ; ++i)
    {
        surface_elevation_for_each_point_in_mesh[(size_t)i] = wave_height((double)OP.m(0,i),(double)OP.m(1,i),t);
        relative_wave_height_for_each_point_in_mesh[(size_t)i] = (double)OP.m(2,i) - surface_elevation_for_each_point_in_mesh[(size_t)i];
    }
}

double SurfaceElevationInterface::evaluate_rao(
        const double , //!< x-position of the RAO's calculation point in the NED frame (in meters)
        const double , //!< y-position of the RAO's calculation point in the NED frame (in meters)
        const double , //!< Current time instant (in seconds)
        const std::vector<std::vector<std::vector<double> > >& , //!< Module of the RAO
        const std::vector<std::vector<std::vector<double> > >&  //!< Phase of the RAO
        ) const
{
    return 0;
}

ssc::kinematics::Point SurfaceElevationInterface::orbital_velocity(
        const double ,   //!< gravity (in m/s^2)
        const double ,   //!< x-position in the NED frame (in meters)
        const double ,   //!< y-position in the NED frame (in meters)
        const double ,   //!< z-position in the NED frame (in meters)
        const double     //!< Current time instant (in seconds)
        ) const
{
    return ssc::kinematics::Point("NED", 0, 0, 0);
}

std::vector<std::vector<double> > SurfaceElevationInterface::get_wave_directions_for_each_model() const
{
    return std::vector<std::vector<double> >();
}

std::vector<std::vector<double> > SurfaceElevationInterface::get_wave_angular_frequency_for_each_model() const
{
    return std::vector<std::vector<double> >();
}

double SurfaceElevationInterface::get_dynamic_pressure(
        const double rho,                           //!< Water density (in kg/m^3)
        const double g,                             //!< Gravity (in m/s^2)
        const ssc::kinematics::Point& P,            //!< Position of point P, relative to the centre of the NED frame, but projected in any frame
        const ssc::kinematics::KinematicsPtr& k,    //!< Object used to compute the transforms to the NED frame
        const double eta,                           //!< Wave elevation at P in the NED frame (in meters)
        const double t                              //!< Current instant (in seconds)
        ) const
{
    const ssc::kinematics::Point OP = compute_relative_position(P, k);
    return dynamic_pressure(rho, g, OP.x(),OP.y(),OP.z(),eta,t);
}

ssc::kinematics::PointMatrixPtr SurfaceElevationInterface::get_output_mesh_in_NED_frame(const ssc::kinematics::KinematicsPtr& k) const
{
    if (output_mesh->get_frame() != "NED")
    {
        return ssc::kinematics::PointMatrixPtr(new ssc::kinematics::PointMatrix(k->get(output_mesh->get_frame(),"NED")*(*output_mesh)));
    }
    return output_mesh;
}

std::vector<ssc::kinematics::Point> SurfaceElevationInterface::get_waves_on_mesh(
        const ssc::kinematics::KinematicsPtr& k,    //!< Object used to compute the transforms to the NED frame
        const double t //!< Current instant (in seconds)
        ) const
{
    if (output_mesh->m.cols()==0) return std::vector<ssc::kinematics::Point>();
    return get_points_on_free_surface(t, get_output_mesh_in_NED_frame(k));
}

SurfaceElevationGrid SurfaceElevationInterface::get_waves_on_mesh_as_a_grid(
        const ssc::kinematics::KinematicsPtr& k, //!< Object used to compute the transforms to the NED frame
        const double t //!< Current instant (in seconds)
        ) const
{
    std::vector<ssc::kinematics::Point> res = get_waves_on_mesh(k,t);
    if (res.empty()) return SurfaceElevationGrid(t);
    const size_t nx = output_mesh_size.first;
    const size_t ny = output_mesh_size.second;
    if ((nx*ny)!=res.size())
    {
        std::stringstream ss;
        ss << "Problem : " <<std::endl
           << "nx*ny = "<<nx << "*"<<ny <<"="<<nx*ny
           <<" should be equal to res.size() = "<<res.size()<<std::endl;
        THROW(__PRETTY_FUNCTION__,ssc::exception_handling::Exception,ss.str());
    }
    SurfaceElevationGrid s(nx,ny,t);
    for(size_t i=0;i<nx;++i)
    {
        s.x(i) = res.at(i).v(0);
    }
    for(size_t j=0; j<ny; ++j)
    {
        s.y(j) = res.at(j*nx).v(1);
    }
    size_t idx = 0;
    for(size_t j=0;j<ny;++j)
    {
        for(size_t i = 0;i<nx;++i)
        {
            s.z(i,j) = res.at(idx++).v(2);
        }
    }
    return s;
}
