/*
 * maneuvering_compilerTest.cpp
 *
 *  Created on: Jan 22, 2015
 *      Author: cady
 */

#include "maneuvering_parserTest.hpp"
#include "NumericalEvaluator.hpp"
#include "StringEvaluator.hpp"

using namespace maneuvering;

maneuvering_parserTest::maneuvering_parserTest() :
            a(ssc::random_data_generator::DataGenerator(2121212188)),
            states(),
            ds(),
            t(a.random<double>()),
            g()
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

maneuvering_parserTest::~maneuvering_parserTest()
{
}

void maneuvering_parserTest::SetUp()
{
}

void maneuvering_parserTest::TearDown()
{
}

using boost::spirit::ascii::blank;

TEST_F(maneuvering_parserTest, parse_very_simple_grammar)
{
    const std::string s = "1.2 2.3 3.4";
    std::string::const_iterator b = s.begin(), e = s.end();
    std::vector<double> v;
    maneuvering::simple_grammar sg;
    qi::phrase_parse(b, e, sg.values_for_tests, blank, v);
    ASSERT_EQ(3, v.size());
    ASSERT_DOUBLE_EQ(1.2, v.at(0));
    ASSERT_DOUBLE_EQ(2.3, v.at(1));
    ASSERT_DOUBLE_EQ(3.4, v.at(2));
}

double numerical_parse(const std::string& string_to_parse);
double numerical_parse(const std::string& string_to_parse)
{
    std::string::const_iterator b = string_to_parse.begin(), e = string_to_parse.end();
    Expr ast;
    ArithmeticGrammar g;
    qi::phrase_parse(b, e, g.expr, blank, ast);
    NumericalEvaluator visitor;
    return visitor(ast);
}

std::string string_parse(const std::string& string_to_parse);
std::string string_parse(const std::string& string_to_parse)
{
    std::string::const_iterator b = string_to_parse.begin(), e = string_to_parse.end();
    Expr ast;
    ArithmeticGrammar g;
    qi::phrase_parse(b, e, g.expr, blank, ast);
    std::stringstream ss;
    StringEvaluator visitor(ss);
    visitor(ast);
    return ss.str();
}

TEST_F(maneuvering_parserTest, arithmetic_parser_can_parse_numbers)
{
    ASSERT_DOUBLE_EQ(0,      numerical_parse("-0"));
    ASSERT_DOUBLE_EQ(0,      numerical_parse("0"));
    ASSERT_DOUBLE_EQ(1.2,    numerical_parse("1.2"));
    ASSERT_DOUBLE_EQ(-1.2,   numerical_parse("-1.2"));
    ASSERT_DOUBLE_EQ(1.2e3,  numerical_parse("1.2e3"));
    ASSERT_DOUBLE_EQ(-1.2e3, numerical_parse("-1.2e3"));
}

TEST_F(maneuvering_parserTest, arithmetic_parser_can_parse_power)
{
    ASSERT_DOUBLE_EQ(8,      numerical_parse("2^3"));
}

TEST_F(maneuvering_parserTest, arithmetic_parser_can_parse_multiplications)
{
    ASSERT_DOUBLE_EQ(240,      numerical_parse("1.2*2e2"));
}

TEST_F(maneuvering_parserTest, arithmetic_parser_can_parse_additions)
{
    ASSERT_DOUBLE_EQ(1e3,      numerical_parse("1.2e3-2e2"));
}

TEST_F(maneuvering_parserTest, arithmetic_parser_can_parse_divisions)
{
    ASSERT_DOUBLE_EQ(10,      numerical_parse("1.2e3/1.2e2"));
}

TEST_F(maneuvering_parserTest, complex_expression_for_arithmetic_parser)
{
    ASSERT_DOUBLE_EQ((10.3/4-5.8*5.8*5.8)*std::sqrt(2),      numerical_parse("(10.3/4.-5.8^3)*2^(1/2)"));
}

TEST_F(maneuvering_parserTest, string_evaluation_for_arithmetic_parser)
{
    ASSERT_EQ("10.3",   string_parse("10.3"));
    ASSERT_EQ("10.3/(2)", string_parse("10.3/2"));
    ASSERT_EQ("10.3*(2)", string_parse("10.3*2"));
    ASSERT_EQ("10.3+2", string_parse("10.3+2"));
    ASSERT_EQ("10.3-2", string_parse("10.3-2"));
}

TEST_F(maneuvering_parserTest, can_parse_valid_identifier)
{
    const std::string s = "valid_identifier";
    std::string::const_iterator b = s.begin(), e = s.end();
    std::string actual;
    qi::phrase_parse(b, e, *(g.identifier), blank, actual);
    const std::string expected = s;
    ASSERT_EQ(expected, actual);
}

TEST_F(maneuvering_parserTest, cannot_parse_invalid_identifier)
{
    const std::string s = "0valid_identifier";
    std::string::const_iterator b = s.begin(), e = s.end();
    std::string actual;
    qi::phrase_parse(b, e, *(g.identifier), blank, actual);
    const std::string expected = "";
    ASSERT_EQ(expected, actual);
}

TEST_F(maneuvering_parserTest, can_parse_constant)
{
    const std::string s = "1.234";
    std::string::const_iterator b = s.begin(), e = s.end();
    double actual;
    qi::phrase_parse(b, e, g.constant, blank, actual);
    ASSERT_DOUBLE_EQ(1.234, actual);
}

TEST_F(maneuvering_parserTest, can_parse_functional)
{
    const std::string s = "f(a)";
    std::string::const_iterator b = s.begin(), e = s.end();
    maneuvering::Functional actual;
    qi::phrase_parse(b, e, g.functional, blank, actual);
    ASSERT_EQ("f", actual.identifier);
}

class AtomVisitor
    : public boost::static_visitor<std::string>
{
    public:
        std::string operator()(const maneuvering::Functional & f)
        {
            COUT(f.identifier);
            return f.identifier;
        }

        std::string operator()(const maneuvering::Nil & ) const
        {
            COUT("");
            return "";
        }

        std::string operator()(const maneuvering::Identifier & id) const
        {
            COUT(id);
            return id;
        }

        std::string operator()(const double & d)
        {
            COUT(d);
            return "";
        }

    private:
};

class TermVisitor
    : public boost::static_visitor<double>
{
    public:
        template <typename T> double operator()(const T& ) const
        {
            return 0;
        }
};
template <> double TermVisitor::operator()(const Atom& a) const;
template <> double TermVisitor::operator()(const Atom& a) const
{
COUT("");
    AtomVisitor visitor;
    boost::apply_visitor(visitor, a);
    return boost::get<double>(a);
}

TEST_F(maneuvering_parserTest, can_parse_atom)
{
    const std::string s = "foo(a)";
    const std::string s2 = "1.4";
    std::string::const_iterator b = s.begin(), e = s.end();
    maneuvering::Atom actual;
    qi::phrase_parse(b, e, g.functional, blank, actual);
    AtomVisitor visitor;
    ASSERT_EQ("foo", boost::apply_visitor(visitor, actual));
}

TEST_F(maneuvering_parserTest, can_parse_factor)
{
    const std::string s = "(2.3)";
    std::string::const_iterator b = s.begin(), e = s.end();
    maneuvering::Term actual;
    maneuvering::grammar g;
    qi::phrase_parse(b, e, g.factor, blank, actual);
    const TermVisitor visitor;
    ASSERT_DOUBLE_EQ(2.3, boost::apply_visitor(visitor, actual));
}

TEST_F(maneuvering_parserTest, can_parse_add_expression)
{
    const std::string s = "2.3  + 3.4";
    std::string::const_iterator b = s.begin(), e = s.end();
    maneuvering::Expression expression;
    maneuvering::grammar g;
    qi::phrase_parse(b, e, g.add_expression, blank, expression);
    ASSERT_EQ("+", expression.operator_name);
    const TermVisitor visitor;
    ASSERT_DOUBLE_EQ(2.3, boost::apply_visitor(visitor, expression.lhs));
    ASSERT_DOUBLE_EQ(3.4, boost::apply_visitor(visitor, expression.rhs));
}

TEST_F(maneuvering_parserTest, can_parse_add_expression2)
{
    const std::string s = "2.3  + (3.4)";
    std::string::const_iterator b = s.begin(), e = s.end();
    maneuvering::Expression expression;
    maneuvering::grammar g;
    qi::phrase_parse(b, e, g.add_expression, blank, expression);
    ASSERT_EQ("+", expression.operator_name);
    const TermVisitor visitor;
    ASSERT_DOUBLE_EQ(2.3, boost::apply_visitor(visitor, expression.lhs));
    ASSERT_DOUBLE_EQ(3.4, boost::apply_visitor(visitor, expression.rhs));
}

TEST_F(maneuvering_parserTest, can_parse_mul_expression)
{
    const std::string s = "2.3    * 3.4";
    std::string::const_iterator b = s.begin(), e = s.end();
    maneuvering::Expression expression;
    maneuvering::grammar g;
    qi::phrase_parse(b, e, g.mul_expression, blank, expression);
    ASSERT_EQ("*", expression.operator_name);
    const TermVisitor visitor;
    ASSERT_DOUBLE_EQ(2.3, boost::apply_visitor(visitor, expression.lhs));
    ASSERT_DOUBLE_EQ(3.4, boost::apply_visitor(visitor, expression.rhs));
}

TEST_F(maneuvering_parserTest, can_parse_mul_expression2)
{
    const std::string s = "2.3    ^ (3.4-x)";
    std::string::const_iterator b = s.begin(), e = s.end();
    maneuvering::Expression expression;
    maneuvering::grammar g;
    qi::phrase_parse(b, e, g.mul_expression, blank, expression);
    ASSERT_EQ("^", expression.operator_name);
    const TermVisitor visit_term;
    ASSERT_DOUBLE_EQ(2.3, boost::apply_visitor(visit_term, expression.lhs));
}

TEST_F(maneuvering_parserTest, can_parse_expression)
{
    const std::string s = "2.3";
    std::string::const_iterator b = s.begin(), e = s.end();
    maneuvering::Term expression;
    maneuvering::grammar g;
    qi::phrase_parse(b, e, g.expression, blank, expression);
    const TermVisitor visit_term;
    ASSERT_DOUBLE_EQ(2.3, boost::apply_visitor(visit_term, expression));
}
