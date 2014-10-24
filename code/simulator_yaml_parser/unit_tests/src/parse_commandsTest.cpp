/*
 * parse_commandsTest.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#include "parse_commandsTest.hpp"
#include "parse_commands.hpp"
#include "yaml_data.hpp"

parse_commandsTest::parse_commandsTest() : a(ssc::random_data_generator::DataGenerator(542121))
{
}

parse_commandsTest::~parse_commandsTest()
{
}

void parse_commandsTest::SetUp()
{
}

void parse_commandsTest::TearDown()
{
}

TEST_F(parse_commandsTest, example)
{
//! [parse_commandsTest example]
    std::vector<YamlCommands> commands = parse_command_yaml(test_data::controlled_forces());
    ASSERT_EQ(1, commands.size());
    ASSERT_EQ("propeller", commands[0].name);
    ASSERT_EQ(4, commands[0].t.size());
    ASSERT_DOUBLE_EQ(0, commands[0].t[0]);
    ASSERT_DOUBLE_EQ(1, commands[0].t[1]);
    ASSERT_DOUBLE_EQ(3, commands[0].t[2]);
    ASSERT_DOUBLE_EQ(10, commands[0].t[3]);
    ASSERT_EQ(2, commands[0].commands.size());
    ASSERT_EQ(4, commands[0].commands["rpm"].size());
    ASSERT_DOUBLE_EQ(3, commands[0].commands["rpm"][0]);
    ASSERT_DOUBLE_EQ(30, commands[0].commands["rpm"][1]);
    ASSERT_DOUBLE_EQ(30, commands[0].commands["rpm"][2]);
    ASSERT_DOUBLE_EQ(40, commands[0].commands["rpm"][3]);
    ASSERT_EQ(4, commands[0].commands["P/D"].size());
    ASSERT_DOUBLE_EQ(0.7, commands[0].commands["P/D"][0]);
    ASSERT_DOUBLE_EQ(0.7, commands[0].commands["P/D"][1]);
    ASSERT_DOUBLE_EQ(0.7, commands[0].commands["P/D"][2]);
    ASSERT_DOUBLE_EQ(0.7, commands[0].commands["P/D"][3]);
//! [parse_commandsTest example]
}

