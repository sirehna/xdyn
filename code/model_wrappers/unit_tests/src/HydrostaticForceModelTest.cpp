/*
 * HydrostaticForceModuleTest.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include "Body.hpp"
#include "DefaultSurfaceElevation.hpp"
#include "FastHydrostaticForceModel.hpp"
#include "HydrostaticForceModelTest.hpp"
#include "generate_body_for_tests.hpp"
#include "TriMeshTestData.hpp"
#include "MeshIntersector.hpp"
#include "ExactHydrostaticForceModel.hpp"

#include <ssc/kinematics.hpp>

#define BODY "body 1"

HydrostaticForceModelTest::HydrostaticForceModelTest() : a(ssc::random_data_generator::DataGenerator(212154))
{
}

HydrostaticForceModelTest::~HydrostaticForceModelTest()
{
}

void HydrostaticForceModelTest::SetUp()
{
}

void HydrostaticForceModelTest::TearDown()
{
}

TEST_F(HydrostaticForceModelTest, example)
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

    Body body = get_body(BODY, points);
    body.G = G;

    FastHydrostaticForceModel F(env);
    const double t = a.random<double>();
    const std::vector<double> vz = env.w->get_relative_wave_height(body.M,env.k,t);
    body.intersector->update_intersection_with_free_surface(vz);
    const ssc::kinematics::Wrench Fhs = F(body, t);
//! [HydrostaticModuleTest example]
//! [HydrostaticModuleTest expected output]
    const double dz = 2./3;
    const double dS = 4;

    ASSERT_DOUBLE_EQ(env.rho*env.g*dz*dS, Fhs.X());
    ASSERT_DOUBLE_EQ(0, Fhs.Y());
    ASSERT_DOUBLE_EQ(0, Fhs.Z());
    ASSERT_DOUBLE_EQ(0, Fhs.K());
    ASSERT_DOUBLE_EQ(0, Fhs.M());
    ASSERT_DOUBLE_EQ(0, Fhs.N());
}

/**
 * \note Test of a fully immersed rectangle plane with points [P1,P2,P3,P4]
 * P1 = [-2,+4,+6]
 * P2 = [-2,-4,+6]
 * P3 = [+2,+4,+4]
 * P4 = [+2,-4,+4]
 * The rectangle is divided into two triangles
 * T1 = [P1,P2,P3]
 * T2 = [P3,P2,P4]
 *
 * The triangles have the same area: \f$8\sqrt(5)\f$
 *
 * The triangles have the same unit normal:
 * \f$[\sin(\atan(0.5)),0,\cos(\atan(0.5))]\f$
 *
 * The immersed volume is 200m3, the norm of the resulting effort should be
 * \f$200*\rho*g\f$.
 *
 * The resulting force evaluated at the origin point O in the global frame (NED)
 * should have no component along axis Y
 *
 * The resulting torque evaluated at the origin point O in the global frame (NED)
 * should have no component around X and Z.
 */
TEST_F(HydrostaticForceModelTest, DISABLED_oriented_fully_immerged_rectangle)
{
    EnvironmentAndFrames env;
    env.g = 9.81;
    env.rho = 1024;
    env.k = ssc::kinematics::KinematicsPtr(new ssc::kinematics::Kinematics());
    const ssc::kinematics::Point G("NED",0,0,0);
    env.k->add(ssc::kinematics::Transform(ssc::kinematics::Point("NED"), "mesh(" BODY ")"));
    env.k->add(ssc::kinematics::Transform(ssc::kinematics::Point("NED"), BODY));
    TR1(shared_ptr)<ssc::kinematics::PointMatrix> mesh;
    env.w = SurfaceElevationPtr(new DefaultSurfaceElevation(0, mesh));

    const auto points = two_triangles_immerged();

    Body body = get_body(BODY, points);
    body.G = G;

    ASSERT_DOUBLE_EQ(0.0, body.x_relative_to_mesh);
    ASSERT_DOUBLE_EQ(0.0, body.y_relative_to_mesh);
    ASSERT_DOUBLE_EQ(0.0, body.z_relative_to_mesh);
    ASSERT_DOUBLE_EQ(1.0, body.mesh_to_body(0,0));
    ASSERT_DOUBLE_EQ(1.0, body.mesh_to_body(1,1));
    ASSERT_DOUBLE_EQ(1.0, body.mesh_to_body(2,2));
    ASSERT_DOUBLE_EQ(0.0, body.mesh_to_body(0,1));
    ASSERT_DOUBLE_EQ(0.0, body.mesh_to_body(0,2));
    ASSERT_DOUBLE_EQ(0.0, body.mesh_to_body(1,0));
    ASSERT_DOUBLE_EQ(0.0, body.mesh_to_body(1,2));
    ASSERT_DOUBLE_EQ(0.0, body.mesh_to_body(2,0));
    ASSERT_DOUBLE_EQ(0.0, body.mesh_to_body(2,1));

    FastHydrostaticForceModel F(env);
    const ssc::kinematics::Wrench Fhs = F(body,a.random<double>());

    ASSERT_EQ(3,(size_t)body.mesh->nodes.rows());
    ASSERT_EQ(4,(size_t)body.mesh->nodes.cols());

    ASSERT_DOUBLE_EQ(-2.0,body.mesh->nodes(0,0));
    ASSERT_DOUBLE_EQ(+4.0,body.mesh->nodes(1,0));
    ASSERT_DOUBLE_EQ(+6.0,body.mesh->nodes(2,0));

    ASSERT_DOUBLE_EQ(-2.0,body.mesh->nodes(0,1));
    ASSERT_DOUBLE_EQ(-4.0,body.mesh->nodes(1,1));
    ASSERT_DOUBLE_EQ(+6.0,body.mesh->nodes(2,1));

    ASSERT_DOUBLE_EQ(+2.0,body.mesh->nodes(0,2));
    ASSERT_DOUBLE_EQ(+4.0,body.mesh->nodes(1,2));
    ASSERT_DOUBLE_EQ(+4.0,body.mesh->nodes(2,2));

    ASSERT_DOUBLE_EQ(+2.0,body.mesh->nodes(0,3));
    ASSERT_DOUBLE_EQ(-4.0,body.mesh->nodes(1,3));
    ASSERT_DOUBLE_EQ(+4.0,body.mesh->nodes(2,3));

    ASSERT_EQ(2,body.mesh->facets.size());
    for (size_t i=0;i<2;++i)
    {
        ASSERT_DOUBLE_EQ(sin(atan(0.5)),body.mesh->facets[i].unit_normal(0));
        ASSERT_DOUBLE_EQ(0.0,body.mesh->facets[i].unit_normal(1));
        ASSERT_DOUBLE_EQ(cos(atan(0.5)),body.mesh->facets[i].unit_normal(2));
    }

    ASSERT_DOUBLE_EQ(-2.0/3.0,body.mesh->facets[0].barycenter(0));
    ASSERT_DOUBLE_EQ(+4.0/3.0,body.mesh->facets[0].barycenter(1));
    ASSERT_DOUBLE_EQ(5.0+1.0/3.0,body.mesh->facets[0].barycenter(2));

    ASSERT_DOUBLE_EQ(+2.0/3.0,body.mesh->facets[1].barycenter(0));
    ASSERT_DOUBLE_EQ(-4.0/3.0,body.mesh->facets[1].barycenter(1));
    ASSERT_DOUBLE_EQ(4.0+2.0/3.0,body.mesh->facets[1].barycenter(2));

    // What is currently implemented with the hypothesis of application
    // point force located at barycenter of each face
    ASSERT_DOUBLE_EQ(-1024*9.81*8*sqrt(5)*sin(atan(0.5))*(5+1.0/3.0+5-1/3.0), Fhs.X());
    ASSERT_DOUBLE_EQ(0.0, Fhs.Y());
    ASSERT_DOUBLE_EQ(-1024*9.81*8*sqrt(5)*cos(atan(0.5))*(5+1.0/3.0+5-1/3.0), Fhs.Z());

    // What is expected with the correct evaluation of application point force
    // All these tests fail.
    ASSERT_DOUBLE_EQ(env.rho*env.g*200.0, sqrt(Fhs.X()*Fhs.X()+Fhs.Z()*Fhs.Z()));
    ASSERT_DOUBLE_EQ(0.0, Fhs.Y());
    ASSERT_DOUBLE_EQ(-env.rho*env.g*200.0*sin(atan(0.5)), Fhs.X());
    ASSERT_DOUBLE_EQ(-env.rho*env.g*200.0*cos(atan(0.5)), Fhs.Z());
    ASSERT_DOUBLE_EQ(0, Fhs.K());
    ASSERT_DOUBLE_EQ(env.rho*env.g*200.0 * (5.0*sin(atan(0.5))+sqrt(5.0)/3.0), Fhs.M());
    ASSERT_DOUBLE_EQ(0, Fhs.N());
}

TEST_F(HydrostaticForceModelTest, potential_energy_half_immersed_cube_fast)
{
    EnvironmentAndFrames env;
    env.g = 9.81;
    env.rho = 1024;
    env.k = ssc::kinematics::KinematicsPtr(new ssc::kinematics::Kinematics());
    env.k->add(ssc::kinematics::Transform(ssc::kinematics::Point("NED"), "mesh(" BODY ")"));
    env.k->add(ssc::kinematics::Transform(ssc::kinematics::Point("NED"), BODY));
    TR1(shared_ptr)<ssc::kinematics::PointMatrix> mesh;
    env.w = SurfaceElevationPtr(new DefaultSurfaceElevation(0, mesh));

    Body body = get_body(BODY, unit_cube());
    std::vector<double> x(13,0);
    std::vector<double> dz;
    for (size_t i = 0 ; i < 4 ; ++i) dz.push_back(0.5);
    for (size_t i = 0 ; i < 4 ; ++i) dz.push_back(-0.5);
    FastHydrostaticForceModel F(env);
    body.intersector->update_intersection_with_free_surface(dz);
    const double Ep = F.potential_energy(body, x);
    ASSERT_DOUBLE_EQ(-1024*0.5*9.81*0.25, Ep);
}

TEST_F(HydrostaticForceModelTest, potential_energy_half_immersed_cube_exact)
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

    Body body = get_body(BODY, unit_cube());
    std::vector<double> x(13,0);
    std::vector<double> dz;
    for (size_t i = 0 ; i < 4 ; ++i) dz.push_back(0.5);
    for (size_t i = 0 ; i < 4 ; ++i) dz.push_back(-0.5);
    body.intersector->update_intersection_with_free_surface(dz);

    ExactHydrostaticForceModel F(env);
    const double Ep = F.potential_energy(body, x);
    ASSERT_DOUBLE_EQ(-1024*0.5*9.81*0.25, Ep);
}
