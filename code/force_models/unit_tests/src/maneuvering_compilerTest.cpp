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

using boost::spirit::ascii::blank;

TEST_F(maneuvering_compilerTest, can_compile_constant)
{
    const auto f = compile(ConstantNode(1.23456));
    ASSERT_EQ(1.23456, f->get_lambda()(states, ds, t));
}


TEST_F(maneuvering_compilerTest, parse_very_simple_grammar)
{
    const std::string s = "1.2 2.3 3.4";
    std::string::const_iterator b = s.begin(), e = s.end();
    std::vector<double> v;
    maneuvering::simple_grammar g;
    qi::phrase_parse(b, e, g.values_for_tests, blank, v);
    ASSERT_EQ(3, v.size());
    ASSERT_DOUBLE_EQ(1.2, v.at(0));
    ASSERT_DOUBLE_EQ(2.3, v.at(1));
    ASSERT_DOUBLE_EQ(3.4, v.at(2));
}

TEST_F(maneuvering_compilerTest, can_parse_valid_identifier)
{
    const std::string s = "valid_identifier";
    std::string::const_iterator b = s.begin(), e = s.end();
    std::string actual;
    maneuvering::grammar g;
    qi::phrase_parse(b, e, *(g.identifier), blank, actual);
    const std::string expected = s;
    ASSERT_EQ(expected, actual);
}

TEST_F(maneuvering_compilerTest, cannot_parse_invalid_identifier)
{
    const std::string s = "0valid_identifier";
    std::string::const_iterator b = s.begin(), e = s.end();
    std::string actual;
    maneuvering::grammar g;
    qi::phrase_parse(b, e, *(g.identifier), blank, actual);
    const std::string expected = "";
    ASSERT_EQ(expected, actual);
}

TEST_F(maneuvering_compilerTest, can_parse_constant)
{
    const std::string s = "1.234";
    std::string::const_iterator b = s.begin(), e = s.end();
    double actual;
    maneuvering::grammar g;
    qi::phrase_parse(b, e, g.constant, blank, actual);
    ASSERT_DOUBLE_EQ(1.234, actual);
}

TEST_F(maneuvering_compilerTest, can_parse_functional)
{
    const std::string s = "f(a)";
    std::string::const_iterator b = s.begin(), e = s.end();
    maneuvering::Functional actual;
    maneuvering::grammar g;
    qi::phrase_parse(b, e, g.functional, blank, actual);
    ASSERT_EQ("f", actual.identifier);
}

class IsFunction
    : public boost::static_visitor<>
{
    public:
        IsFunction() : function_name(""){}
        void operator()(const maneuvering::Functional & f)
        {
            COUT(f.identifier);
            function_name = f.identifier;
        }

        void operator()(const maneuvering::Nil & ) const
        {
            COUT("");
        }

        void operator()(const maneuvering::Identifier & id) const
        {
            COUT(id);
        }

        void operator()(const double & d) const
        {
            COUT(d);
        }

        bool matches(const std::string& name) const
        {
            return function_name==name;
        }

    private:

        std::string function_name;

};

TEST_F(maneuvering_compilerTest, can_parse_atom)
{
    const std::string s = "foo(a)";
    const std::string s2 = "1.4";
    std::string::const_iterator b = s.begin(), e = s.end();
    maneuvering::Atom actual;
    maneuvering::grammar g;
    qi::phrase_parse(b, e, g.functional, blank, actual);
    IsFunction parsed_function_name;
    boost::apply_visitor(parsed_function_name, actual);
    ASSERT_TRUE(parsed_function_name.matches("foo"));
}
