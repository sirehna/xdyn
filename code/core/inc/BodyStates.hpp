/*
 * BodyStates.hpp
 *
 *  Created on: Jan 8, 2015
 *      Author: cady
 */

#ifndef BODYSTATES_HPP_
#define BODYSTATES_HPP_

#include <tuple>

#include <ssc/macros.hpp>
#include TR1INC(memory)

#include <Eigen/Dense>

#include <ssc/kinematics.hpp>
#include "ForceModel.hpp"
#include "GeometricTypes3d.hpp"
#include "MeshIntersector.hpp"
#include "StateMacros.hpp"
#include "History.hpp"

struct YamlRotation;


class Mesh;
typedef TR1(shared_ptr)<Mesh> MeshPtr;
typedef TR1(shared_ptr)<Eigen::Matrix<double,6,6> > MatrixPtr;

struct BodyStates
{
    BodyStates();
    std::string name;                                              //!< Body's name
    ssc::kinematics::Point G;                                      //!< Position of the ship's centre of gravity
    double m;                                                      //!< Total mass of the body (in kg)
    MeshPtr mesh;                                                  //!< Vertices & edges of the body's mesh
    MatrixPtr total_inertia;                                       //!< 6x6 matrix corresponding to the sum of the rigid body inertia + added mass expressed in the body frame
    MatrixPtr solid_body_inertia;                                  //!< 6x6 rigid body inertia matrix (i.e. without added mass) in the body frame
    MatrixPtr inverse_of_the_total_inertia;
    double x_relative_to_mesh;                                     //!< Position of the body frame relative to the mesh frame, along the x-axis, in meters
    double y_relative_to_mesh;                                     //!< Position of the body frame relative to the mesh frame, along the y-axis, in meters
    double z_relative_to_mesh;                                     //!< Position of the body frame relative to the mesh frame, along the z-axis, in meters
    ssc::kinematics::RotationMatrix mesh_to_body;                  //!< Rotation matrix from mesh to body
    ssc::kinematics::PointMatrixPtr M;                             //!< For the wave model (just the points in the mesh)
    History x;                                                     //!< x-coordinate of the body's center of gravity in the NED frame (in m)
    History y;                                                     //!< y-coordinate of the body's center of gravity in the NED frame (in m)
    History z;                                                     //!< z-coordinate of the body's center of gravity in the NED frame (in m)
    History u;                                                     //!< Projection of the body's translation speed (relative to NED) along the body's X-axis (in m/s)
    History v;                                                     //!< Projection of the body's translation speed (relative to NED) along the body's Y-axis (in m/s)
    History w;                                                     //!< Projection of the body's translation speed (relative to NED) along the body's Z-axis (in m/s)
    History p;                                                     //!< Projection of the body's rotational speed (relative to NED) along the body's X-axis (in rad/s)
    History q;                                                     //!< Projection of the body's rotational speed (relative to NED) along the body's Y-axis (in rad/s)
    History r;                                                     //!< Projection of the body's rotational speed (relative to NED) along the body's Z-axis (in rad/s)
    History qr;                                                    //!< Real part of the quaternion (of the rotation from NED to body)
    History qi;                                                    //!< Imaginary part of the quaternion (of the rotation from NED to body)
    History qj;                                                    //!< Imaginary part of the quaternion (of the rotation from NED to body)
    History qk;                                                    //!< Imaginary part of the quaternion (of the rotation from NED to body)
    MeshIntersectorPtr intersector;                                //!< Allows us to iterate on all emerged or immersed facets
    EPoint down_direction_in_mesh_frame;                           //!< Unit vertical vector, expressed in the body's mesh frame
    ssc::kinematics::Point hydrodynamic_forces_calculation_point;  //!< Point of expression of hydrodynamic forces (except Froude-Krylov & hydrostatic)

    ssc::kinematics::EulerAngles get_angles(const YamlRotation& c) const;
    ssc::kinematics::EulerAngles get_angles(const StateType& all_states, const size_t idx, const YamlRotation& c) const;
    ssc::kinematics::EulerAngles convert(const ssc::kinematics::RotationMatrix& R, const YamlRotation& rotations) const;
    std::tuple<double,double,double,double> convert(const ssc::kinematics::EulerAngles& R, const YamlRotation& rotations) const;
    ssc::kinematics::RotationMatrix get_rot_from_ned_to_body() const;
    ssc::kinematics::RotationMatrix get_rot_from_ned_to(const StateType& x, const size_t idx) const;
};

#endif /* BODYSTATES_HPP_ */
