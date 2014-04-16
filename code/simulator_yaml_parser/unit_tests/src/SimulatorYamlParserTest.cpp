/*
 * SimulatorYamlParserTest.cpp
 *
 *  Created on: 15 avr. 2014
 *      Author: cady
 */


#include "SimulatorYamlParserTest.hpp"
#include "YamlRotation.hpp"
#include "yaml_data.hpp"
#include "SimulatorYamlParser.hpp"

SimulatorYamlParserTest::SimulatorYamlParserTest() : a(DataGenerator(1212))
{
}

SimulatorYamlParserTest::~SimulatorYamlParserTest()
{
}

void SimulatorYamlParserTest::SetUp()
{
}

void SimulatorYamlParserTest::TearDown()
{
}

TEST_F(SimulatorYamlParserTest, can_parse_rotations)
{
    const YamlRotation rot = SimulatorYamlParser(test_data::rotation()).get_rotation();
    ASSERT_EQ("angle", rot.order_by);
    ASSERT_EQ(3, rot.convention.size());
    ASSERT_EQ("x", rot.convention.at(0));
    ASSERT_EQ("y'", rot.convention.at(1));
    ASSERT_EQ("z''", rot.convention.at(2));
}

TEST_F(SimulatorYamlParserTest, can_parse_models)
{
    const YamlModel mod1 = SimulatorYamlParser(test_data::rotation()).parse_model(test_data::model1());
    ASSERT_EQ("no waves", mod1.model);
    ASSERT_EQ("frame: NED\nmodel: no waves\nz:\n  unit: m\n  value: 0", mod1.yaml);
    const YamlModel mod2 = SimulatorYamlParser(test_data::rotation()).parse_model(test_data::model2());
    ASSERT_EQ("gravity", mod2.model);
    ASSERT_EQ("g:\n  unit: m/s^2\n  value: 9.81\nmodel: gravity", mod2.yaml);
}

TEST_F(SimulatorYamlParserTest, DISABLED_can_parse_coordinates)
{
    const bool implemented = false;
    ASSERT_TRUE(implemented);
}
