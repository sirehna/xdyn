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

using boost::spirit::ascii::space;

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
    if (not(f.matrix_sections.empty())) os << "Sections:\n";
    for (auto it = f.matrix_sections.begin() ; it != f.matrix_sections.end() ; ++it)
    {
        os << "[" << it->header << "] = ";
        for (size_t i = 0 ; i < it->values.size() ; ++i)
        {
            os << "[";
            for (size_t j = 0 ; j < it->values.at(i).size() ; ++j)
            {
                os << it->values.at(i).at(j);
                if (i < it->values.at(i).size()-1) os << ",";
            }
            os << "]" << std::endl;
        }
    }
    if (not(f.sections_with_id.empty())) os << "Sections with ID:\n";
    for (auto it = f.sections_with_id.begin() ; it != f.sections_with_id.end() ; ++it)
    {
        os << "[" << it->header << "] = ";
        for (size_t i = 0 ; i < it->values.size() ; ++i)
        {
            os << "[";
            for (size_t j = 0 ; j < it->values.at(i).size() ; ++j)
            {
                os << it->values.at(i).at(j);
                if (i < it->values.at(i).size()-1) os << ",";
            }
            os << "]" << std::endl;
        }
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
    const std::string s = "[sdgf sdfgsdgf sdfgsdg]";
    std::string::const_iterator b = s.begin(), e = s.end();
    std::string actual;
    qi::phrase_parse(b, e, g.header,space,actual);
    ASSERT_EQ("sdgf sdfgsdgf sdfgsdg",actual);
}


TEST_F(HDBParserTest, can_parse_section)
{
    hdb::grammar g;
    const std::string s = " [List_calculated_periods]\n"
                          "   1.00\n"
                          "   2.00\n"
                          "   3.00\n"
                          "   3.50\n"
                          "   3.80\n"
                          "   4.00\n";
    std::string::const_iterator b = s.begin(), e = s.end();
    hdb::VectorSection section;
    qi::phrase_parse(b, e, g.vector_section,space,section);

    ASSERT_EQ("List_calculated_periods", section.header);
    ASSERT_EQ(6,section.values.size());
    ASSERT_DOUBLE_EQ(1,   section.values.at(0));
    ASSERT_DOUBLE_EQ(2,   section.values.at(1));
    ASSERT_DOUBLE_EQ(3,   section.values.at(2));
    ASSERT_DOUBLE_EQ(3.5, section.values.at(3));
    ASSERT_DOUBLE_EQ(3.8, section.values.at(4));
    ASSERT_DOUBLE_EQ(4,   section.values.at(5));
}

TEST_F(HDBParserTest, can_parse_two_sections)
{
    hdb::grammar g;
    const std::string s = " [List_calculated_periods]\n"
                          "   1.00\n"
                          "   2.00\n"
                          "   3.00\n"
                          "   3.50\n"
                          "   3.80\n"
                          "   4.00\n"
                          " [List_calculated_headings]\n"
                          "   0.000000    \n"
                          "   15.00000    \n"
                          "   30.00000    \n"
                          "   45.00000    \n"
                          "   60.00000    \n"
                          "   75.00000    \n"
                          "   90.00000    \n"
                          "   105.0000    \n"
                          "   120.0000    \n"
                          "   135.0000    \n"
                          "   150.0000    \n"
                          "   165.0000    \n"
                          "   180.0000    \n";
    std::string::const_iterator b = s.begin(), e = s.end();
    std::vector<hdb::VectorSection> sections;
    qi::phrase_parse(b, e, *(g.vector_section),space,sections);
    ASSERT_EQ(2,sections.size());
    ASSERT_EQ("List_calculated_periods", sections.at(0).header);
    ASSERT_EQ("List_calculated_headings", sections.at(1).header);
    ASSERT_EQ(6,sections.at(0).values.size());
    ASSERT_EQ(13,sections.at(1).values.size());
    ASSERT_DOUBLE_EQ(75,sections.at(1).values.at(5));
}

TEST_F(HDBParserTest, can_parse_string)
{
    hdb::grammar g;
    const std::string s = " sdfgsdfg 456ggf 4ggffs  ";
    std::string::const_iterator b = s.begin(), e = s.end();
    std::string string;
    qi::phrase_parse(b, e, g.str,space,string);
    ASSERT_EQ("sdfgsdfg 456ggf 4ggffs", string);
}

TEST_F(HDBParserTest, can_parse_one_string_keys)
{
    hdb::grammar g;
    const std::string s = "[key 1] value 1\n";
    std::string::const_iterator b = s.begin(), e = s.end();
    std::vector<hdb::Key<std::string> > string_keys;
    qi::phrase_parse(b, e, *(g.string_key),space,string_keys);
    ASSERT_EQ(1,string_keys.size());
    ASSERT_EQ("key 1",string_keys.at(0).header);
    ASSERT_EQ("value 1",string_keys.at(0).value);
}

TEST_F(HDBParserTest, can_parse_several_string_keys)
{
    hdb::grammar g;
    const std::string s = "[   key 1 ]     value 1\n"
                          "[  key 2     ]   value 2    \n";
    std::string::const_iterator b = s.begin(), e = s.end();
    std::vector<hdb::Key<std::string> > string_keys;
    qi::phrase_parse(b, e, *(g.string_key),space,string_keys);
    ASSERT_EQ(2,string_keys.size());
    ASSERT_EQ("key 1",string_keys.at(0).header);
    ASSERT_EQ("value 1",string_keys.at(0).value);
    ASSERT_EQ("key 2",string_keys.at(1).header);
    ASSERT_EQ("value 2",string_keys.at(1).value);
}
