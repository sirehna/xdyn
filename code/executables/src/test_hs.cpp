/*
 * test_hs.cpp
 *
 *  Created on: Oct 6, 2014
 *      Author: cady
 */

#include "Body.hpp"
#include "DefaultSurfaceElevation.hpp"
#include "FastHydrostaticForceModel.hpp"
#include "TriMeshTestData.hpp"
#include "MeshIntersector.hpp"
#include "YamlRotation.hpp"
#include "BodyBuilder.hpp"
#include "GeometricTypes3d.hpp"
#include <ssc/kinematics.hpp>
#include "generate_anthineas.hpp"
#include <ssc/macros.hpp>
#define BODY "body 1"

#define N 2000000

Body get_body(const std::string& name);
Body get_body(const std::string& name, const VectorOfVectorOfPoints& points);

Body get_body(const std::string& name)
{
    return get_body(name, two_triangles());
}

Body get_body(const std::string& name, const VectorOfVectorOfPoints& points)

{
    YamlRotation rot;
    rot.convention.push_back("z");
    rot.convention.push_back("y'");
    rot.convention.push_back("x''");
    rot.order_by = "angle";
    return BodyBuilder(rot).build(name, points);
}

int main()
{
    EnvironmentAndFrames env;
    env.g = 9.81;
    env.rho = 1024;
    env.k = ssc::kinematics::KinematicsPtr(new ssc::kinematics::Kinematics());
    const ssc::kinematics::Point G("NED",0,2,2./3.);
    env.k->add(ssc::kinematics::Transform(ssc::kinematics::Point("NED"), "mesh(" BODY ")"));
    env.k->add(ssc::kinematics::Transform(ssc::kinematics::Point("NED"), BODY));
    TR1(shared_ptr)<ssc::kinematics::PointMatrix> mesh;
    env.w = SurfaceElevationPtr(new DefaultSurfaceElevation(0, mesh));

    auto points = two_triangles();
    for (size_t i = 0 ; i < 2 ; ++i)
    {
        for (size_t j = 0 ; j < 3 ; ++j)
        {
            double x = points[i][j][0];
            points[i][j][0] = points[i][j][2];
            points[i][j][2] = points[i][j][1];
            points[i][j][1] = x;
        }
    }

    Body body = get_body(BODY, anthineas());
    body.G = G;

    FastHydrostaticForceModel F(env);
    const double t = 0;
    const std::vector<double> vz = env.w->get_relative_wave_height(body.M,env.k,t);
    body.intersector->update_intersection_with_free_surface(vz);
    ssc::kinematics::Wrench Fhs;
    for (size_t i = 0 ; i < N ; ++i) Fhs = F(body, t);
    return 0;
}
