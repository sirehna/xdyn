/*
 * BodyBuilderTest.cpp
 *
 *  Created on: Jun 18, 2014
 *      Author: cady
 */

#include "BodyBuilder.hpp"
#include "BodyBuilderTest.hpp"
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
