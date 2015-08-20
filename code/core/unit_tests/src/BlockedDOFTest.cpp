/*
 * BlockedDOFTest.cpp
 *
 *  Created on: Aug 19, 2015
 *      Author: cady
 */


#include "BlockedDOF.hpp"
#include "BlockedDOFException.hpp"
#include "BlockedDOFTest.hpp"
#include "gmock/gmock.h"
using namespace testing; // So we can use 'ElementsAre' unqualified
#include "SimulatorYamlParserException.hpp"

BlockedDOFTest::BlockedDOFTest() : a(ssc::random_data_generator::DataGenerator(854512))
{
}

BlockedDOFTest::~BlockedDOFTest()
{
}

void BlockedDOFTest::SetUp()
{
}

void BlockedDOFTest::TearDown()
{
}

TEST_F(BlockedDOFTest, can_parse_forced_dof)
{
    const std::string yaml = "from CSV:\n"
                             "  - state: u\n"
                             "    t: T\n"
                             "    value: PS\n"
                             "    interpolation: spline\n"
                             "    filename: test.csv\n"
                             "from YAML:\n"
                             "  - state: p\n"
                             "    t: [4.2]\n"
                             "    value: [5]\n"
                             "    interpolation: piecewise constant\n";
    const BlockedDOF::Yaml input = BlockedDOF::parse(yaml);
    ASSERT_EQ(1, input.from_yaml.size());
    ASSERT_EQ(BlockedDOF::BlockableState::P, input.from_yaml.front().state);
    ASSERT_EQ(BlockedDOF::InterpolationType::PIECEWISE_CONSTANT, input.from_yaml.front().interpolation);
    ASSERT_THAT(input.from_yaml.front().t, ElementsAre(4.2));
    ASSERT_THAT(input.from_yaml.front().value, ElementsAre(5));
    ASSERT_EQ(1, input.from_csv.size());
    ASSERT_EQ(BlockedDOF::BlockableState::U, input.from_csv.front().state);
    ASSERT_EQ(BlockedDOF::InterpolationType::SPLINE, input.from_csv.front().interpolation);
    ASSERT_EQ("T", input.from_csv.front().t);
    ASSERT_EQ("PS", input.from_csv.front().value);
    ASSERT_EQ("test.csv", input.from_csv.front().filename);
}

TEST_F(BlockedDOFTest, should_throw_if_forcing_anything_other_than_uvwpqr)
{
    const std::string yaml = "from CSV:\n"
                             "  - state: x\n"
                             "    t: T\n"
                             "    value: PS\n"
                             "    interpolation: spline\n"
                             "    filename: test.csv\n"
                             "from YAML:\n"
                             "  - state: p\n"
                             "    t: [4.2]\n"
                             "    value: [5]\n"
                             "    interpolation: piecewise constant\n";
    ASSERT_THROW(BlockedDOF::parse(yaml), SimulatorYamlParserException);
}

TEST_F(BlockedDOFTest, should_throw_if_forcing_same_state_twice)
{
    const std::string yaml1 = "from CSV:\n"
                             "  - state: u\n"
                             "    t: T\n"
                             "    value: PS\n"
                             "    interpolation: spline\n"
                             "    filename: test.csv\n"
                             "  - state: u\n"
                             "    t: T\n"
                             "    value: PS\n"
                             "    interpolation: spline\n"
                             "    filename: test.csv\n";
    const std::string yaml2 =
                             "from YAML:\n"
                             "  - state: p\n"
                             "    t: [4.2]\n"
                             "    value: [5]\n"
                             "    interpolation: piecewise constant\n"
                             "  - state: p\n"
                             "    t: [4.2]\n"
                             "    value: [5]\n"
                             "    interpolation: piecewise constant\n";
    const std::string yaml3 = "from CSV:\n"
                             "  - state: u\n"
                             "    t: T\n"
                             "    value: PS\n"
                             "    interpolation: spline\n"
                             "    filename: test.csv\n"
                             "from YAML:\n"
                             "  - state: u\n"
                             "    t: [4.2]\n"
                             "    value: [5]\n"
                             "    interpolation: piecewise constant\n";

    ASSERT_THROW(BlockedDOF(BlockedDOF::parse(yaml1)), BlockedDOFException);
    ASSERT_THROW(BlockedDOF(BlockedDOF::parse(yaml2)), BlockedDOFException);
    ASSERT_THROW(BlockedDOF(BlockedDOF::parse(yaml3)), BlockedDOFException);
}

TEST_F(BlockedDOFTest, should_throw_if_not_as_many_values_as_instants)
{
    const std::string invalid_yaml =
                             "from YAML:\n"
                             "  - state: p\n"
                             "    t: [4.2,5]\n"
                             "    value: [5]\n"
                             "    interpolation: piecewise constant\n";
    ASSERT_THROW(BlockedDOF::Builder(BlockedDOF::parse(invalid_yaml)).get_forced_states(), BlockedDOFException);
}

TEST_F(BlockedDOFTest, DISABLED_should_throw_if_t_not_strictly_increasing)
{
    ASSERT_TRUE(false);
}

TEST_F(BlockedDOFTest, DISABLED_interpolation_type_should_be_valid)
{
    ASSERT_TRUE(false);
}

TEST_F(BlockedDOFTest, DISABLED_should_throw_if_CSV_file_does_not_exist)
{
    ASSERT_TRUE(false);
}

TEST_F(BlockedDOFTest, DISABLED_piecewise_constant_should_work)
{
    ASSERT_TRUE(false);
}

TEST_F(BlockedDOFTest, DISABLED_linear_should_work)
{
    ASSERT_TRUE(false);
}

TEST_F(BlockedDOFTest, DISABLED_spline_should_work)
{
    ASSERT_TRUE(false);
}

TEST_F(BlockedDOFTest, DISABLED_blocked_derivative_should_work)
{
    ASSERT_TRUE(false);
}

TEST_F(BlockedDOFTest, DISABLED_force_delta)
{
    ASSERT_TRUE(false);
}
