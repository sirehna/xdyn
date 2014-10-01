/*
 * Body.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#ifndef BODY_HPP_
#define BODY_HPP_

#include <ssc/macros.hpp>
#include TR1INC(memory)

#include <Eigen/Dense>

#include <ssc/kinematics.hpp>

class Mesh;
class PointMatrix;
typedef TR1(shared_ptr)<Mesh> MeshPtr;
typedef TR1(shared_ptr)<Eigen::Matrix<double,6,6> > MatrixPtr;
typedef TR1(shared_ptr)<ssc::kinematics::PointMatrix> PointMatrixPtr;

class YamlBody;

struct Body
{
    Body();
    std::string name;                             //!< Body's name
    ssc::kinematics::Point G;                     //!< Position of the ship's centre of gravity
    double m;                                     //!< Total mass of the body (in kg)
    MeshPtr mesh;                                 //!< Vertices & edges of the body's mesh
    MatrixPtr total_inertia;                      //!< 6x6 matrix corresponding to the sum of the rigid body inertia + added mass expressed in the body frame
    MatrixPtr solid_body_inertia;                 //!< 6x6 rigid body inertia matrix (i.e. without added mass) in the body frame
    MatrixPtr inverse_of_the_total_inertia;       //!< 6x6 matrix corresponding to the inverse of the sum of the rigid body inertia + added mass expressed in the body frame
    double x_relative_to_mesh;                    //!< Position of the body frame relative to the mesh frame, along the x-axis, in meters
    double y_relative_to_mesh;                    //!< Position of the body frame relative to the mesh frame, along the y-axis, in meters
    double z_relative_to_mesh;                    //!< Position of the body frame relative to the mesh frame, along the z-axis, in meters
    ssc::kinematics::RotationMatrix mesh_to_body; //!< Rotation matrix from mesh to body
    PointMatrixPtr M;                             //!< For the wave model (just the points in the mesh)
    double u;                                     //!< Projection of the body's translation speed (relative to NED) along the body's X-axis (in m/s)
    double v;                                     //!< Projection of the body's translation speed (relative to NED) along the body's Y-axis (in m/s)
    double w;                                     //!< Projection of the body's translation speed (relative to NED) along the body's Z-axis (in m/s)
    double p;                                     //!< Projection of the body's rotational speed (relative to NED) along the body's X-axis (in rad/s)
    double q;                                     //!< Projection of the body's rotational speed (relative to NED) along the body's Y-axis (in rad/s)
    double r;                                     //!< Projection of the body's rotational speed (relative to NED) along the body's Z-axis (in rad/s)
};

#endif /* BODY_HPP_ */
