/*
 * Body.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#ifndef BODY_HPP_
#define BODY_HPP_

#include "tr1_macros.hpp"
#include TR1INC(memory)

#include <Eigen/Dense>

#include "RotationMatrix.hpp"
#include "ForceModel.hpp"
#include "GeometricTypes3d.hpp"

class Mesh;
class PointMatrix;
typedef TR1(shared_ptr)<Mesh> MeshPtr;
typedef TR1(shared_ptr)<Eigen::Matrix<double,6,6> > MatrixPtr;
typedef TR1(shared_ptr)<PointMatrix> PointMatrixPtr;

class YamlBody;

struct Body
{
    Body();
    std::string name;
    Point G;
    double m;
    MeshPtr mesh;
    MatrixPtr total_inertia;
    MatrixPtr solid_body_inertia;
    MatrixPtr inverse_of_the_total_inertia;
    double x_relative_to_mesh;
    double y_relative_to_mesh;
    double z_relative_to_mesh;
    RotationMatrix mesh_to_body;
    PointMatrixPtr M; //<! For the wave model (just the points in the mesh)
};


#endif /* BODY_HPP_ */
