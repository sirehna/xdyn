/*
 * maneuvering_compilerTest.cpp
 *
 *  Created on: Jan 22, 2015
 *      Author: cady
 */

#include "maneuvering_parserTest.hpp"
#include "maneuvering_grammar.hpp"
#include "NumericalEvaluator.hpp"
#include "StringEvaluator.hpp"

using namespace maneuvering;

maneuvering_parserTest::maneuvering_parserTest() :
            a(ssc::random_data_generator::DataGenerator(2121212188))
{
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
    std::string::const_iterator it = string_to_parse.begin(), e = string_to_parse.end();
    Expr ast;
    ArithmeticGrammar g;
    const bool success = qi::phrase_parse(it, e, g.expr, blank, ast);
    if (not(success and it == e))
    {
        return "NA";
    }
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

TEST_F(maneuvering_parserTest, simple_function_call_for_arithmetic_parser)
{
    ASSERT_DOUBLE_EQ(2,      numerical_parse("sqrt(4)"));
}

TEST_F(maneuvering_parserTest, function_calls_for_arithmetic_parser)
{
    ASSERT_DOUBLE_EQ(5,      numerical_parse("abs(sqrt(3^2+4^2))"));
}

TEST_F(maneuvering_parserTest, string_evaluation_for_arithmetic_parser)
{
    ASSERT_EQ("10.3",   string_parse("10.3"));
    ASSERT_EQ("10.3/(2)", string_parse("10.3/2"));
    ASSERT_EQ("10.3*(2)", string_parse("10.3*2"));
    ASSERT_EQ("10.3+2", string_parse("10.3+2"));
    ASSERT_EQ("10.3-2", string_parse("10.3-2"));
    ASSERT_EQ("fabs", string_parse("fabs"));
}

TEST_F(maneuvering_parserTest, can_parse_valid_identifier)
{
    ASSERT_EQ("valid_identifier",   string_parse("valid_identifier"));
    ASSERT_EQ("Var_1",   string_parse("Var_1"));
}

TEST_F(maneuvering_parserTest, cannot_parse_invalid_identifier)
{
    const std::string invalid_identifier = "0valid_identifier";
    ASSERT_EQ("NA",   string_parse(invalid_identifier));
}
