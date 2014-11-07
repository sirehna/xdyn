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



