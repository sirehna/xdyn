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

double evaluate(const maneuvering::Function& f);
double evaluate(const maneuvering::Function& f)
{
    BodyStates states;
    ssc::data_source::DataSource ds;
    const double t = 0;
    return f(states, ds, t);
}

TEST_F(maneuvering_compilerTest, example)
{
    const auto n = maneuvering::compile("2");
    const auto f = n->get_lambda();
    ASSERT_DOUBLE_EQ(2, evaluate(f));
}



