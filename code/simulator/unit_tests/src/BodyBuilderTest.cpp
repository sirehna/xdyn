/*
 * BodyBuilderTest.cpp
 *
 *  Created on: Jun 18, 2014
 *      Author: cady
 */

#include "BodyBuilder.hpp"
#include "BodyBuilderTest.hpp"
#include "Mesh.hpp"
#include "SimulatorYamlParser.hpp"
#include "STL_data.hpp"
#include "StlReader.hpp"
#include "yaml_data.hpp"
#include "PointMatrix.hpp"

BodyBuilderTest::BodyBuilderTest() : a(DataGenerator(1218221)), body(build_body())
{
}

BodyBuilderTest::~BodyBuilderTest()
{
}

void BodyBuilderTest::SetUp()
{
}

void BodyBuilderTest::TearDown()
{
}

Body BodyBuilderTest::build_body()
{
    const auto yaml = SimulatorYamlParser(test_data::full_example()).parse();
    const auto mesh = read_stl(test_data::cube());
    BodyBuilder builder(yaml.rotations);
    return builder.build(yaml.bodies.front(), mesh);
}

TEST_F(BodyBuilderTest, name_should_be_correct)
{
    ASSERT_EQ("body 1", body.name);
}
TEST_F(BodyBuilderTest, centre_of_gravity_should_be_computed_properly)
{
    const auto G = body.G;
    ASSERT_EQ("body 1", G.get_frame());
    ASSERT_DOUBLE_EQ(4, G.x);
    ASSERT_DOUBLE_EQ(7, G.y);
    ASSERT_DOUBLE_EQ(-10, G.z);
}

TEST_F(BodyBuilderTest, mass_should_be_correct)
{
    ASSERT_DOUBLE_EQ(1E6, body.m);
}

TEST_F(BodyBuilderTest, relative_position_should_be_correct)
{
    ASSERT_DOUBLE_EQ(10,body.x_relative_to_mesh);
    ASSERT_DOUBLE_EQ(0.21,body.y_relative_to_mesh);
    ASSERT_DOUBLE_EQ(33E3,body.z_relative_to_mesh);
}

TEST_F(BodyBuilderTest, mesh_is_correct)
{
    ASSERT_EQ(12,body.mesh->facets.size());
    for (size_t i = 0 ; i < 12 ; ++i)
    {
        ASSERT_DOUBLE_EQ(0.5,body.mesh->facets.at(i).area) << std::endl << "Facet: " << i << " (starting at 0)";
    }
    ASSERT_DOUBLE_EQ(0,(double)body.mesh->facets.at(0).unit_normal(0));
    ASSERT_DOUBLE_EQ(0,(double)body.mesh->facets.at(0).unit_normal(1));
    ASSERT_DOUBLE_EQ(1,(double)body.mesh->facets.at(0).unit_normal(2));
    ASSERT_EQ(8,body.mesh->nodes.cols());
    ASSERT_EQ(3,body.mesh->nodes.rows());
    ASSERT_EQ(-0.5,(double)body.mesh->nodes.col(0)(0));
    ASSERT_EQ(-0.5,(double)body.mesh->nodes.col(0)(1));
    ASSERT_EQ(1,(double)body.mesh->nodes.col(0)(2));
}

TEST_F(BodyBuilderTest, mesh_to_body_is_correct)
{
    ASSERT_DOUBLE_EQ(0.41198224566568298,     (double)body.mesh_to_body(0,0));
    ASSERT_DOUBLE_EQ(-0.90019762973551742391, (double)body.mesh_to_body(1,0));
    ASSERT_DOUBLE_EQ(-0.14112000805986721352, (double)body.mesh_to_body(2,0));
    ASSERT_DOUBLE_EQ(-0.54071226417559081767, (double)body.mesh_to_body(0,1));
    ASSERT_DOUBLE_EQ(-0.11686748793698308047, (double)body.mesh_to_body(1,1));
    ASSERT_DOUBLE_EQ(-0.83304996106680495593, (double)body.mesh_to_body(2,1));
    ASSERT_DOUBLE_EQ(0.73341725956399950181,  (double)body.mesh_to_body(0,2));
    ASSERT_DOUBLE_EQ(0.41950711279054053726,  (double)body.mesh_to_body(1,2));
    ASSERT_DOUBLE_EQ(-0.53489522870537720145, (double)body.mesh_to_body(2,2));
}

TEST_F(BodyBuilderTest, rigid_body_inertia_is_correct)
{
    ASSERT_DOUBLE_EQ(1,(double)body.solid_body_inertia->operator()(0,0));
    ASSERT_DOUBLE_EQ(2,(double)body.solid_body_inertia->operator()(0,1));
    ASSERT_DOUBLE_EQ(3,(double)body.solid_body_inertia->operator()(0,2));
    ASSERT_DOUBLE_EQ(4,(double)body.solid_body_inertia->operator()(0,3));
    ASSERT_DOUBLE_EQ(5,(double)body.solid_body_inertia->operator()(0,4));
    ASSERT_DOUBLE_EQ(6,(double)body.solid_body_inertia->operator()(0,5));
    ASSERT_DOUBLE_EQ(7,(double)body.solid_body_inertia->operator()(1,0));
    ASSERT_DOUBLE_EQ(2,(double)body.solid_body_inertia->operator()(1,1));
    ASSERT_DOUBLE_EQ(1,(double)body.solid_body_inertia->operator()(1,2));
    ASSERT_DOUBLE_EQ(2,(double)body.solid_body_inertia->operator()(1,3));
    ASSERT_DOUBLE_EQ(11,(double)body.solid_body_inertia->operator()(1,4));
    ASSERT_DOUBLE_EQ(13,(double)body.solid_body_inertia->operator()(1,5));
    ASSERT_DOUBLE_EQ(20,(double)body.solid_body_inertia->operator()(2,0));
    ASSERT_DOUBLE_EQ(30,(double)body.solid_body_inertia->operator()(2,1));
    ASSERT_DOUBLE_EQ(3,(double)body.solid_body_inertia->operator()(2,2));
    ASSERT_DOUBLE_EQ(80,(double)body.solid_body_inertia->operator()(2,3));
    ASSERT_DOUBLE_EQ(0.9,(double)body.solid_body_inertia->operator()(2,4));
    ASSERT_DOUBLE_EQ(0.1,(double)body.solid_body_inertia->operator()(2,5));
}

TEST_F(BodyBuilderTest, total_inertia_is_correct)
{
    ASSERT_DOUBLE_EQ(2,(double)body.total_inertia->operator()(0,0));
    ASSERT_DOUBLE_EQ(4,(double)body.total_inertia->operator()(0,1));
    ASSERT_DOUBLE_EQ(6,(double)body.total_inertia->operator()(0,2));
    ASSERT_DOUBLE_EQ(8,(double)body.total_inertia->operator()(0,3));
    ASSERT_DOUBLE_EQ(10,(double)body.total_inertia->operator()(0,4));
    ASSERT_DOUBLE_EQ(12,(double)body.total_inertia->operator()(0,5));
    ASSERT_DOUBLE_EQ(0,(double)body.total_inertia->operator()(1,0));
    ASSERT_DOUBLE_EQ(4,(double)body.total_inertia->operator()(1,1));
    ASSERT_DOUBLE_EQ(2,(double)body.total_inertia->operator()(1,2));
    ASSERT_DOUBLE_EQ(4,(double)body.total_inertia->operator()(1,3));
    ASSERT_DOUBLE_EQ(22,(double)body.total_inertia->operator()(1,4));
    ASSERT_DOUBLE_EQ(26,(double)body.total_inertia->operator()(1,5));
    ASSERT_DOUBLE_EQ(0,(double)body.total_inertia->operator()(2,0));
    ASSERT_DOUBLE_EQ(60,(double)body.total_inertia->operator()(2,1));
    ASSERT_DOUBLE_EQ(6,(double)body.total_inertia->operator()(2,2));
    ASSERT_DOUBLE_EQ(160,(double)body.total_inertia->operator()(2,3));
    ASSERT_DOUBLE_EQ(1.8,(double)body.total_inertia->operator()(2,4));
    ASSERT_DOUBLE_EQ(0.2,(double)body.total_inertia->operator()(2,5));
}

TEST_F(BodyBuilderTest, mesh_should_be_correct)
{
    ASSERT_EQ("mesh(body 1)", body.M->get_frame());
    ASSERT_DOUBLE_EQ(-0.5, (double)body.M->m(0,0));
    ASSERT_DOUBLE_EQ(-0.5, (double)body.M->m(1,0));
    ASSERT_DOUBLE_EQ(   1, (double)body.M->m(2,0));

    ASSERT_DOUBLE_EQ( 0.5, (double)body.M->m(0,1));
    ASSERT_DOUBLE_EQ(-0.5, (double)body.M->m(1,1));
    ASSERT_DOUBLE_EQ(   1, (double)body.M->m(2,1));

    ASSERT_DOUBLE_EQ( 0.5, (double)body.M->m(0,2));
    ASSERT_DOUBLE_EQ( 0.5, (double)body.M->m(1,2));
    ASSERT_DOUBLE_EQ(   1, (double)body.M->m(2,2));

    ASSERT_DOUBLE_EQ(-0.5, (double)body.M->m(0,3));
    ASSERT_DOUBLE_EQ( 0.5, (double)body.M->m(1,3));
    ASSERT_DOUBLE_EQ(   1, (double)body.M->m(2,3));

    ASSERT_DOUBLE_EQ( 0.5, (double)body.M->m(0,4));
    ASSERT_DOUBLE_EQ(-0.5, (double)body.M->m(1,4));
    ASSERT_DOUBLE_EQ(   0, (double)body.M->m(2,4));

    ASSERT_DOUBLE_EQ(-0.5, (double)body.M->m(0,5));
    ASSERT_DOUBLE_EQ(-0.5, (double)body.M->m(1,5));
    ASSERT_DOUBLE_EQ(   0, (double)body.M->m(2,5));

    ASSERT_DOUBLE_EQ(-0.5, (double)body.M->m(0,6));
    ASSERT_DOUBLE_EQ( 0.5, (double)body.M->m(1,6));
    ASSERT_DOUBLE_EQ(   0, (double)body.M->m(2,6));

    ASSERT_DOUBLE_EQ( 0.5, (double)body.M->m(0,7));
    ASSERT_DOUBLE_EQ( 0.5, (double)body.M->m(1,7));
    ASSERT_DOUBLE_EQ(   0, (double)body.M->m(2,7));
}
