/*
 * maneuvering_compilerTest.cpp
 *
 *  Created on: Jan 22, 2015
 *      Author: cady
 */

#include "maneuvering_compilerTest.hpp"
#include "maneuvering_compiler.hpp"
#include "maneuvering_grammar.hpp"

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


TEST_F(maneuvering_compilerTest, parse_very_simple_grammar)
{
    std::string s = "1.2 2.3 3.4";
    std::string::const_iterator b = s.begin(), e = s.end();
    std::vector<double> v;
    maneuvering::grammar g;
    using boost::spirit::ascii::blank;
    qi::phrase_parse(b, e, g.values_for_tests, blank, v);
    ASSERT_EQ(3, v.size());
    ASSERT_DOUBLE_EQ(1.2, v.at(0));
    ASSERT_DOUBLE_EQ(2.3, v.at(1));
    ASSERT_DOUBLE_EQ(3.4, v.at(2));
}
//
//TEST_F(maneuvering_compilerTest, can_parse_sum)
//{
//    std::string s = "1.2+2.3";
//    std::string::const_iterator b = s.begin(), e = s.end();
//    std::vector<double> v;
//    maneuvering::grammar g;
//    using boost::spirit::ascii::blank;
//    qi::phrase_parse(b, e, g.values, blank, v);
//    ASSERT_EQ(3, v.size());
//    ASSERT_DOUBLE_EQ(1.2, v.at(0));
//    ASSERT_DOUBLE_EQ(2.2, v.at(1));
//    ASSERT_DOUBLE_EQ(3.4, v.at(2));
//}
