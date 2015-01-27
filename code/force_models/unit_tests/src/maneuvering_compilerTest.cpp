/*
 * maneuvering_compilerTest.cpp
 *
 *  Created on: Jan 23, 2015
 *      Author: cady
 */


#include "maneuvering_compilerTest.hpp"
#include "maneuvering_compiler.hpp"

maneuvering_compilerTest::maneuvering_compilerTest() : a(ssc::random_data_generator::DataGenerator(2121545))
{
}

maneuvering_compilerTest::~maneuvering_compilerTest()
{
}

void maneuvering_compilerTest::SetUp()
{
}

void maneuvering_compilerTest::TearDown()
{
}

double evaluate(const std::string& stuff, const BodyStates& states, const double t, ssc::data_source::DataSource& ds);
double evaluate(const std::string& stuff, const BodyStates& states, const double t, ssc::data_source::DataSource& ds)
{
    const auto n = maneuvering::compile(stuff);
    const auto f = n->get_lambda();
    return f(states, ds, t);
}

double test_compile(const std::string& stuff);
double test_compile(const std::string& stuff)
{
    BodyStates states;
    ssc::data_source::DataSource ds;
    const double t = 0;
    return evaluate(stuff, states, t, ds);
}

TEST_F(maneuvering_compilerTest, can_compile_constant)
{
    ASSERT_DOUBLE_EQ(0,         test_compile("0"));
    ASSERT_DOUBLE_EQ(2,         test_compile("2"));
    ASSERT_DOUBLE_EQ(1.234e-6,  test_compile("1.234e-6"));
    ASSERT_DOUBLE_EQ(-1.234e-6, test_compile("-1.234e-6"));
}

TEST_F(maneuvering_compilerTest, can_compile_sum)
{
    ASSERT_DOUBLE_EQ(3,    test_compile("1+2"));
    ASSERT_DOUBLE_EQ(0.79, test_compile("-1.21+2"));
}

TEST_F(maneuvering_compilerTest, can_compile_product)
{
    ASSERT_DOUBLE_EQ(24,    test_compile("20*1.2"));
}

TEST_F(maneuvering_compilerTest, can_compile_cosine)
{
    ASSERT_DOUBLE_EQ(1,    test_compile("cos(0)"));
}

TEST_F(maneuvering_compilerTest, can_compile_sine)
{
    ASSERT_DOUBLE_EQ(sin(1),    test_compile("sin(1)"));
}

TEST_F(maneuvering_compilerTest, can_compile_exp_and_log)
{
    ASSERT_DOUBLE_EQ(1,    test_compile("exp(log(1))"));
    ASSERT_DOUBLE_EQ(10,   test_compile("exp(log(10))"));
}

TEST_F(maneuvering_compilerTest, can_compile_sqrt)
{
    ASSERT_DOUBLE_EQ(5,    test_compile("sqrt(25)"));
}

TEST_F(maneuvering_compilerTest, can_compile_abs)
{
    ASSERT_DOUBLE_EQ(2.5e3,    test_compile("abs(-2.5e3)"));
}
