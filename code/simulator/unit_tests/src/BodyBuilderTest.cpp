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

Body BodyBuilderTest::build_body() const
{
    const auto yaml = SimulatorYamlParser(test_data::full_example()).parse();
    const auto mesh = read_stl(test_data::cube());
    BodyBuilder builder(yaml.rotations);
    return builder.build(yaml.bodies.front(), mesh);
}

TEST_F(BodyBuilderTest, name_should_be_correct)
{
    const auto body = build_body();
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
    const auto body = build_body();
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
        ASSERT_DOUBLE_EQ(0.5,body.mesh->facets.at(i).area);
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
