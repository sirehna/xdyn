/*
 * maneuvering_compilerTest.cpp
 *
 *  Created on: Jan 22, 2015
 *      Author: cady
 */

#include "maneuvering_compilerTest.hpp"
#include "maneuvering_compiler.hpp"

using namespace maneuvering;

maneuvering_compilerTest::maneuvering_compilerTest() :
            a(ssc::random_data_generator::DataGenerator(2121212188)),
            states(),
            ds(),
            t(a.random<double>())
{
    states.x.record(0, a.random<double>());
    states.y.record(0, a.random<double>());
    states.z.record(0, a.random<double>());
    states.u.record(0, a.random<double>());
    states.v.record(0, a.random<double>());
    states.w.record(0, a.random<double>());
    states.p.record(0, a.random<double>());
    states.q.record(0, a.random<double>());
    states.r.record(0, a.random<double>());
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

TEST_F(maneuvering_compilerTest, can_compile_constant)
{
    const auto f = compile(ConstantNode(1.23456));
    ASSERT_EQ(1.23456, f->get_lambda()(states, ds, t));
}

