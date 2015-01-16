/*
 * parse_outputTest.cpp
 *
 *  Created on: Jan 13, 2015
 *      Author: cady
 */


#include "parse_outputTest.hpp"
#include "parse_output.hpp"
#include "yaml_data.hpp"

parse_outputTest::parse_outputTest() : a(ssc::random_data_generator::DataGenerator(215451))
{
}

parse_outputTest::~parse_outputTest()
{
}

void parse_outputTest::SetUp()
{
}

void parse_outputTest::TearDown()
{
}

TEST_F(parse_outputTest, can_parse_outputs)
{
    const auto res = parse_output(test_data::full_example());
    ASSERT_EQ(2, res.size());
    ASSERT_EQ("csv", res.at(0).format);
    ASSERT_EQ("tutorial_1.csv", res.at(0).filename);
    ASSERT_EQ(1, res.at(0).data.size());
    ASSERT_EQ("x(body 1)", res.at(0).data.at(0));

    ASSERT_EQ(4, res.at(1).data.size());
    ASSERT_EQ("hdf5", res.at(1).format);
    ASSERT_EQ("tutorial_1.h5", res.at(1).filename);
    ASSERT_EQ("y(body 1)", res.at(1).data.at(0));
    ASSERT_EQ("z(body 1)", res.at(1).data.at(1));
    ASSERT_EQ("Fz(gravity,body 1)", res.at(1).data.at(2));
    ASSERT_EQ("wave", res.at(1).data.at(3));
}

TEST_F(parse_outputTest, should_work_even_if_string_is_empty)
{
    parse_output("");
    ASSERT_NO_THROW(parse_output(""));
}
