/*
 * HDBParserTest.cpp
 *
 *  Created on: Nov 5, 2014
 *      Author: cady
 */

#include "HDBParser.hpp"
#include "HDBParserTest.hpp"
#include "hdb_data.hpp"
#include "hdb_to_ast.hpp"
#include "hdb_parser_internal_data_structures.hpp"
#include "hdb_grammar.hpp"

HDBParserTest::HDBParserTest() : a(ssc::random_data_generator::DataGenerator(212))
{
}

HDBParserTest::~HDBParserTest()
{
}

void HDBParserTest::SetUp()
{
}

void HDBParserTest::TearDown()
{
}

std::ostream& operator<<(std::ostream& os, const hdb::AST& f);
std::ostream& operator<<(std::ostream& os, const hdb::AST& f)
{
    if (not(f.value_keys.empty())) os << "Value keys:\n";
    for (auto it = f.value_keys.begin() ; it != f.value_keys.end() ; ++it)
    {
        os << "[" << it->header << "] = " << it->value << std::endl;
    }
    if (not(f.string_keys.empty())) os << "String keys:\n";
    for (auto it = f.string_keys.begin() ; it != f.string_keys.end() ; ++it)
    {
        os << "[" << it->header << "] = " << it->value << std::endl;
    }
    if (not(f.sections.empty())) os << "Sections:\n";
    for (auto it = f.sections.begin() ; it != f.sections.end() ; ++it)
    {
        os << "[" << it->header << "] = [";
        for (size_t i = 0 ; i < it->values.size() ; ++i)
        {
            os << it->values.at(i);
            if (i < it->values.size()-1) os << ",";
        }
        os << "]" << std::endl;
    }
    if (not(f.sections_with_id.empty())) os << "Sections with ID:\n";
    for (auto it = f.sections_with_id.begin() ; it != f.sections_with_id.end() ; ++it)
    {
        os << "[" << it->header << "] [" << it->id << "] = [";
        for (size_t i = 0 ; i < it->values.size() ; ++i)
        {
            os << it->values.at(i);
            if (i < it->values.size()-1) os << ",";
        }
        os << "]" << std::endl;
    }
    return os;
}

TEST_F(HDBParserTest, can_parse_string_key)
{
    const hdb::AST hdb_file = hdb::parse("[test this] string");
    ASSERT_EQ(1,hdb_file.string_keys.size());
    ASSERT_EQ("test this",hdb_file.string_keys.at(0).header);
    ASSERT_EQ("string",hdb_file.string_keys.at(0).value);
}

TEST_F(HDBParserTest, can_parse_value_key)
{
    const hdb::AST hdb_file = hdb::parse("[FORWARD SPEED]   1.23");
    ASSERT_EQ(1,hdb_file.value_keys.size());
    ASSERT_EQ("FORWARD SPEED",hdb_file.value_keys.at(0).header);
    ASSERT_EQ(1.23,hdb_file.value_keys.at(0).value);
}

TEST_F(HDBParserTest, can_parse_list_of_doubles)
{
    hdb::grammar g;
    using boost::spirit::ascii::space;
    const std::string s = "12.1 12.3456789";
    std::string::const_iterator b = s.begin(), e = s.end();
    std::vector<double> d;
    const bool match = qi::phrase_parse(b, e, g.values,space,d);
    ASSERT_EQ(2,d.size());
    ASSERT_DOUBLE_EQ(12.1,d.front());
    ASSERT_DOUBLE_EQ(12.3456789,d.back());
}

TEST_F(HDBParserTest, can_parse_key)
{
    hdb::grammar g;
    using boost::spirit::ascii::space;
    const std::string s = "[sdgf sdfgsdgf sdfgsdg]";
    std::string::const_iterator b = s.begin(), e = s.end();
    std::string actual;
    const bool match = qi::phrase_parse(b, e, g.header,space,actual);
    ASSERT_EQ("sdgf sdfgsdgf sdfgsdg",actual);
}
