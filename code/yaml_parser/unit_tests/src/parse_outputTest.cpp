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
    ASSERT_EQ("Fz(gravity,body 1,body 1)", res.at(1).data.at(2));
    ASSERT_EQ("waves", res.at(1).data.at(3));
}

TEST_F(parse_outputTest, should_work_even_if_string_is_empty)
{
    parse_output("");
    ASSERT_NO_THROW(parse_output(""));
}

TEST_F(parse_outputTest, can_create_list_of_all_outputs_for_a_given_yaml)
{
    const YamlOutput res = generate_default_outputter_with_all_states_in_it(test_data::full_example(), "");
    ASSERT_EQ("", res.filename);
    ASSERT_EQ("tsv", res.format);
    ASSERT_EQ(13+1+3,res.data.size());
    ASSERT_EQ("t", res.data.at(0));
    ASSERT_EQ("x(body 1)", res.data.at(1));
    ASSERT_EQ("y(body 1)", res.data.at(2));
    ASSERT_EQ("z(body 1)", res.data.at(3));
    ASSERT_EQ("u(body 1)", res.data.at(4));
    ASSERT_EQ("v(body 1)", res.data.at(5));
    ASSERT_EQ("w(body 1)", res.data.at(6));
    ASSERT_EQ("p(body 1)", res.data.at(7));
    ASSERT_EQ("q(body 1)", res.data.at(8));
    ASSERT_EQ("r(body 1)", res.data.at(9));
    ASSERT_EQ("qr(body 1)", res.data.at(10));
    ASSERT_EQ("qi(body 1)", res.data.at(11));
    ASSERT_EQ("qj(body 1)", res.data.at(12));
    ASSERT_EQ("qk(body 1)", res.data.at(13));
    ASSERT_EQ("phi(body 1)", res.data.at(14));
    ASSERT_EQ("theta(body 1)", res.data.at(15));
    ASSERT_EQ("psi(body 1)", res.data.at(16));
}

TEST_F(parse_outputTest, format_is_HDF5_if_extension_is_h5)
{
    const YamlOutput res = generate_default_outputter_with_all_states_in_it(test_data::full_example(), "blabla.h5");
    ASSERT_EQ("blabla.h5", res.filename);
    ASSERT_EQ("hdf5", res.format);
}

TEST_F(parse_outputTest, format_is_CSV_if_extension_is_csv)
{
    const YamlOutput res = generate_default_outputter_with_all_states_in_it(test_data::full_example(), "blabla.csv");
    ASSERT_EQ("blabla.csv", res.filename);
    ASSERT_EQ("csv", res.format);
}
