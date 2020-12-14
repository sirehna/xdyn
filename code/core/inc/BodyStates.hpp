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
#include "GeometricTypes3d.hpp"
#include "MeshIntersector.hpp"
#include "StateMacros.hpp"
#include "History.hpp"
#include "YamlRotation.hpp"
#include "AbstractStates.hpp"

struct YamlRotation;


class Mesh;
typedef TR1(shared_ptr)<Mesh> MeshPtr;
typedef TR1(shared_ptr)<Eigen::Matrix<double,6,6> > MatrixPtr;

struct BodyStates : AbstractStates<History>
{
    BodyStates(const double Tmax=0 //!< Defines how much history we store
              );
    BodyStates& operator=(const AbstractStates<History>& rhs);
    std::string name;                                              //!< Body's name
    ssc::kinematics::Point G;                                      //!< Position of the ship's centre of gravity
    MeshPtr mesh;                                                  //!< Vertices & edges of the body's mesh
    MatrixPtr total_inertia;                                       //!< 6x6 matrix corresponding to the sum of the rigid body inertia + added mass expressed in the body frame
    MatrixPtr solid_body_inertia;                                  //!< 6x6 rigid body inertia matrix (i.e. without added mass) in the body frame
    MatrixPtr inverse_of_the_total_inertia;
    double x_relative_to_mesh;                                     //!< Position of the body frame relative to the mesh frame, along the x-axis, in meters
    double y_relative_to_mesh;                                     //!< Position of the body frame relative to the mesh frame, along the y-axis, in meters
    double z_relative_to_mesh;                                     //!< Position of the body frame relative to the mesh frame, along the z-axis, in meters
    ssc::kinematics::RotationMatrix mesh_to_body;                  //!< Rotation matrix from mesh to body
    ssc::kinematics::PointMatrixPtr M;                             //!< For the wave model (just the points in the mesh)
    MeshIntersectorPtr intersector;                                //!< Allows us to iterate on all emerged or immersed facets
    EPoint g_in_mesh_frame;                                        //!< Unit vertical vector, expressed in the body's mesh frame
    ssc::kinematics::Point hydrodynamic_forces_calculation_point;  //!< Point of expression of hydrodynamic forces (except Froude-Krylov & hydrostatic)

    ssc::kinematics::EulerAngles get_angles() const;
    ssc::kinematics::EulerAngles get_angles(const YamlRotation& c) const;
    ssc::kinematics::EulerAngles get_angles(const StateType& all_states, const size_t idx, const YamlRotation& c) const;
    static ssc::kinematics::EulerAngles convert(const ssc::kinematics::RotationMatrix& R, const YamlRotation& rotations);
    static std::tuple<double,double,double,double> convert(const ssc::kinematics::EulerAngles& R, const YamlRotation& rotations);
    ssc::kinematics::RotationMatrix get_rot_from_ned_to_body() const;
    ssc::kinematics::RotationMatrix get_rot_from_ned_to(const StateType& x, const size_t idx) const;
    YamlRotation convention;  //!< Rotation convention
    std::vector<double> get_current_state_values(const size_t idx) const;
};

#endif /* BODYSTATES_HPP_ */
