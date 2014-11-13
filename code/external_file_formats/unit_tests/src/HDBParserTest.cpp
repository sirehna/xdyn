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

using boost::spirit::ascii::blank;

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

std::ostream& operator<<(std::ostream& os, const hdb::Key<double>& k);
std::ostream& operator<<(std::ostream& os, const hdb::Key<double>& k)
{
    os << "[" << k.header << "] = " << k.value;
    return os;
}

std::ostream& operator<<(std::ostream& os, const hdb::Key<std::string>& k);
std::ostream& operator<<(std::ostream& os, const hdb::Key<std::string>& k)
{
    os << "[" << k.header << "] = " << k.value;
    return os;
}

std::ostream& operator<<(std::ostream& os, const hdb::Values& k);
std::ostream& operator<<(std::ostream& os, const hdb::Values& k)
{
    os << "[";
    for (size_t i = 0 ; i < k.size()-1 ; ++i)
    {
        os << k.at(i) << ",";
    }
    if (not(k.empty())) os << k.back();
    os << "]";
    return os;
}

std::ostream& operator<<(std::ostream& os, const hdb::MatrixSection& k);
std::ostream& operator<<(std::ostream& os, const hdb::MatrixSection& k)
{
    os << "[" << k.header << "] =\n";
    for (auto it  = k.values.begin() ; it != k.values.end() ; ++it)
    {
        os << '\t' << *it << std::endl;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const hdb::AST& f);
std::ostream& operator<<(std::ostream& os, const hdb::AST& f)
{
    os << "\n*** Value keys: " << f.value_keys.size() << "\n";
    for (auto it = f.value_keys.begin() ; it != f.value_keys.end() ; ++it)
    {
        os << *it << std::endl;
    }
    os << "\n*** String keys: " << f.string_keys.size() << "\n";
    for (auto it = f.string_keys.begin() ; it != f.string_keys.end() ; ++it)
    {
        os << *it << std::endl;
    }
    os << "\n*** Matrix sections: " << f.matrix_sections.size() << "\n";
    for (auto it = f.matrix_sections.begin() ; it != f.matrix_sections.end() ; ++it)
    {
        os << *it << std::endl;
    }
    os << "\n*** Lists of matrix sections: " << f.lists_of_matrix_sections.size() << "\n";
    for (auto it = f.lists_of_matrix_sections.begin() ; it != f.lists_of_matrix_sections.end() ; ++it)
    {
        os << "[" << it->header << "] =\n";
        for (auto it2 = it->sections.begin() ; it2 != it->sections.end() ; ++it2)
        {
            os << '\t' << it2->header << std::endl;
            for (auto it3 = it2->values.begin() ; it3 != it2->values.end() ; ++it3)
            {
                os << "\t\t" << *it3 << std::endl;
            }
        }
    }
    os << "\n*** Lists of matrix sections with ID: " << f.lists_of_matrix_sections_with_id.size() << "\n";
    for (auto it = f.lists_of_matrix_sections_with_id.begin() ; it != f.lists_of_matrix_sections_with_id.end() ; ++it)
    {
        os << "[" << it->header << "] =\n";
        for (auto it2 = it->sections_with_id.begin() ; it2 != it->sections_with_id.end() ; ++it2)
        {
            os << '\t' << it2->header << ", ID: " << it2->id << std::endl;
            for (auto it3 = it2->values.begin() ; it3 != it2->values.end() ; ++it3)
            {
                os << "\t\t" << *it3 << std::endl;
            }
        }
    }
    os << "\n*** Vector sections: " << f.vector_sections.size() << "\n";
    for (auto it = f.vector_sections.begin() ; it != f.vector_sections.end() ; ++it)
    {
        os << "[" << it->header << "] = " << it->values << std::endl;
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
    const std::string s = "12.1 12.3456789";
    std::string::const_iterator b = s.begin(), e = s.end();
    std::vector<double> d;
    const bool match = qi::phrase_parse(b, e, g.values,blank,d);
    ASSERT_EQ(2,d.size());
    ASSERT_DOUBLE_EQ(12.1,d.front());
    ASSERT_DOUBLE_EQ(12.3456789,d.back());
}

TEST_F(HDBParserTest, can_parse_key)
{
    hdb::grammar g;
    const std::string s = "[sdgf sdfgsdgf sdfgsdg]";
    std::string actual;
    qi::phrase_parse(s.begin(), s.end(), g.header,blank,actual);
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
    hdb::VectorSection section;
    qi::phrase_parse(s.begin(), s.end(), g.vector_section,blank,section);

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
    std::vector<hdb::VectorSection> sections;
    qi::phrase_parse(s.begin(), s.end(), *(g.vector_section),blank,sections);
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
    qi::phrase_parse(b, e, g.str,blank,string);
    ASSERT_EQ("sdfgsdfg 456ggf 4ggffs", string);
}

TEST_F(HDBParserTest, can_parse_one_string_keys)
{
    hdb::grammar g;
    const std::string s = "[key 1] value 1\n";
    std::vector<hdb::Key<std::string> > string_keys;
    qi::phrase_parse(s.begin(), s.end(), *(g.string_key),blank,string_keys);
    ASSERT_EQ(1,string_keys.size());
    ASSERT_EQ("key 1",string_keys.at(0).header);
    ASSERT_EQ("value 1",string_keys.at(0).value);
}

TEST_F(HDBParserTest, can_parse_several_string_keys)
{
    hdb::grammar g;
    const std::string s = "[   key 1 ]     value 1\n"
                          "[  key 2     ]   value 2    \n";
    std::vector<hdb::Key<std::string> > string_keys;
    qi::phrase_parse(s.begin(), s.end(), *(g.string_key),blank,string_keys);
    ASSERT_EQ(2,string_keys.size());
    ASSERT_EQ("key 1",string_keys.at(0).header);
    ASSERT_EQ("value 1",string_keys.at(0).value);
    ASSERT_EQ("key 2",string_keys.at(1).header);
    ASSERT_EQ("value 2",string_keys.at(1).value);
}

TEST_F(HDBParserTest, can_parse_a_matrix_section)
{
    hdb::grammar g;
    const std::string s = " [ADDED_MASS_LINE_1]\n"
                          "    1.00  1.097184E+04 -4.534383E+01  1.489058E+04 -6.476872E+01  1.887115E+05  9.087353E+00\n"
                          "    2.00  9.377615E+03 -8.511539E+01  1.576359E+04  3.108509E+00  1.619423E+05  3.926564E+02\n"
                          "    3.00  1.426635E+04 -1.940786E+01  1.841663E+04 -3.755692E+01  1.900012E+05  3.589405E+02\n"
                          "    3.50  1.765988E+04 -2.229278E+01  1.878402E+04 -4.122345E+01  2.292613E+05  3.676993E+02\n"
                          "    3.80  2.066126E+04 -2.218779E+01  1.939457E+04 -4.426564E+01  2.686258E+05  3.974906E+02\n"
                          "    4.00  2.287093E+04 -2.404940E+01  1.989585E+04 -4.781980E+01  2.971014E+05  4.187755E+02\n";

    hdb::MatrixSection m;
    qi::phrase_parse(s.begin(), s.end(), g.matrix_section,blank,m);
    ASSERT_EQ("ADDED_MASS_LINE_1", m.header);
    ASSERT_EQ(6, m.values.size());
    ASSERT_EQ(7, m.values.at(0).size());
    ASSERT_EQ(7, m.values.at(1).size());
    ASSERT_EQ(7, m.values.at(2).size());
    ASSERT_EQ(7, m.values.at(3).size());
    ASSERT_EQ(7, m.values.at(4).size());
    ASSERT_EQ(7, m.values.at(5).size());
    ASSERT_DOUBLE_EQ(1.00, m.values.at(0).at(0));
    ASSERT_DOUBLE_EQ(1.097184E+04, m.values.at(0).at(1));
    ASSERT_DOUBLE_EQ(-4.534383E+01, m.values.at(0).at(2));
    ASSERT_DOUBLE_EQ(1.489058E+04, m.values.at(0).at(3));
    ASSERT_DOUBLE_EQ(-6.476872E+01, m.values.at(0).at(4));
    ASSERT_DOUBLE_EQ(1.887115E+05, m.values.at(0).at(5));
    ASSERT_DOUBLE_EQ(9.087353E+00, m.values.at(0).at(6));

    const auto tree = hdb::parse(s);
    ASSERT_EQ(1,tree.matrix_sections.size());
}

TEST_F(HDBParserTest, can_parse_a_list_of_matrix_sections)
{
    hdb::grammar g;
    const std::string s = " [Added_mass_Radiation_Damping]\n"
                          " [ADDED_MASS_LINE_1]\n"
                          "    1.00  1.097184E+04 -4.534383E+01  1.489058E+04 -6.476872E+01  1.887115E+05  9.087353E+00\n"
                          "    2.00  9.377615E+03 -8.511539E+01  1.576359E+04  3.108509E+00  1.619423E+05  3.926564E+02\n"
                          "    3.00  1.426635E+04 -1.940786E+01  1.841663E+04 -3.755692E+01  1.900012E+05  3.589405E+02\n"
                          "    3.50  1.765988E+04 -2.229278E+01  1.878402E+04 -4.122345E+01  2.292613E+05  3.676993E+02\n"
                          "    3.80  2.066126E+04 -2.218779E+01  1.939457E+04 -4.426564E+01  2.686258E+05  3.974906E+02\n"
                          "    4.00  2.287093E+04 -2.404940E+01  1.989585E+04 -4.781980E+01  2.971014E+05  4.187755E+02\n"
                          " [ADDED_MASS_LINE_2]\n"
                          "    1.00  5.372426E+00  1.039658E+05 -1.038983E+01  1.798326E+04  1.127642E+02  6.116315E+04\n"
                          "    2.00 -2.935182E+01  7.581280E+04  1.907894E+01  2.102286E+04 -2.443126E+02  2.305457E+04\n"
                          "    3.00  8.574823E+00  1.019513E+05  1.553684E+01  2.051240E+04  2.377617E+02  1.078476E+05\n"
                          "    3.50  4.355995E+00  1.434086E+05  1.251381E+00  9.963283E+03  2.165019E+02  1.510785E+05\n"
                          "    3.80  2.883454E+00  1.810525E+05 -1.780741E+01  4.558459E+02  1.719431E+02  1.801648E+05\n"
                          "    4.00  2.647226E+00  2.104009E+05 -2.568808E+01 -8.096014E+03  1.758119E+02  2.022041E+05\n";
    std::vector<hdb::ListOfMatrixSections> lists_of_sections;
    qi::phrase_parse(s.begin(), s.end(), g.list_of_matrix_sections,blank,lists_of_sections);

    hdb::AST ast = hdb::parse(s);

    ASSERT_EQ(1,ast.lists_of_matrix_sections.size());
    ASSERT_EQ(2,ast.lists_of_matrix_sections.at(0).sections.size());

    ASSERT_EQ(1, lists_of_sections.size());
    ASSERT_EQ("Added_mass_Radiation_Damping", lists_of_sections.at(0).header);
    ASSERT_EQ(2, lists_of_sections.at(0).sections.size());
    ASSERT_EQ("ADDED_MASS_LINE_1", lists_of_sections.at(0).sections.at(0).header);
    ASSERT_EQ("ADDED_MASS_LINE_2", lists_of_sections.at(0).sections.at(1).header);
    ASSERT_EQ(6, lists_of_sections.at(0).sections.at(0).values.size());
    ASSERT_EQ(7, lists_of_sections.at(0).sections.at(0).values.at(0).size());
    ASSERT_EQ(7, lists_of_sections.at(0).sections.at(0).values.at(1).size());
    ASSERT_EQ(7, lists_of_sections.at(0).sections.at(0).values.at(2).size());
    ASSERT_EQ(7, lists_of_sections.at(0).sections.at(0).values.at(3).size());
    ASSERT_EQ(7, lists_of_sections.at(0).sections.at(0).values.at(4).size());
    ASSERT_EQ(7, lists_of_sections.at(0).sections.at(0).values.at(5).size());
    ASSERT_EQ(6, lists_of_sections.at(0).sections.at(1).values.size());
    ASSERT_EQ(7, lists_of_sections.at(0).sections.at(1).values.at(0).size());
    ASSERT_EQ(7, lists_of_sections.at(0).sections.at(1).values.at(1).size());
    ASSERT_EQ(7, lists_of_sections.at(0).sections.at(1).values.at(2).size());
    ASSERT_EQ(7, lists_of_sections.at(0).sections.at(1).values.at(3).size());
    ASSERT_EQ(7, lists_of_sections.at(0).sections.at(1).values.at(4).size());
    ASSERT_EQ(7, lists_of_sections.at(0).sections.at(1).values.at(5).size());

    ASSERT_DOUBLE_EQ(3.50, lists_of_sections.at(0).sections.at(1).values.at(3).at(0));
    ASSERT_DOUBLE_EQ(4.355995, lists_of_sections.at(0).sections.at(1).values.at(3).at(1));
    ASSERT_DOUBLE_EQ(1.434086E+05, lists_of_sections.at(0).sections.at(1).values.at(3).at(2));
    ASSERT_DOUBLE_EQ(1.251381, lists_of_sections.at(0).sections.at(1).values.at(3).at(3));
    ASSERT_DOUBLE_EQ(9.963283E+03, lists_of_sections.at(0).sections.at(1).values.at(3).at(4));
    ASSERT_DOUBLE_EQ(2.165019E+02, lists_of_sections.at(0).sections.at(1).values.at(3).at(5));
    ASSERT_DOUBLE_EQ(1.510785E+05, lists_of_sections.at(0).sections.at(1).values.at(3).at(6));
}

TEST_F(HDBParserTest, can_parse_a_list_of_matrix_sections_with_just_one_matrix)
{
    hdb::grammar g;
    const std::string s = " [Added_mass_Radiation_Damping]\n"
                          " [ADDED_MASS_LINE_1]\n"
                          "    1.00  1.097184E+04 -4.534383E+01  1.489058E+04 -6.476872E+01  1.887115E+05  9.087353E+00\n"
                          "    2.00  9.377615E+03 -8.511539E+01  1.576359E+04  3.108509E+00  1.619423E+05  3.926564E+02\n"
                          "    3.00  1.426635E+04 -1.940786E+01  1.841663E+04 -3.755692E+01  1.900012E+05  3.589405E+02\n"
                          "    3.50  1.765988E+04 -2.229278E+01  1.878402E+04 -4.122345E+01  2.292613E+05  3.676993E+02\n"
                          "    3.80  2.066126E+04 -2.218779E+01  1.939457E+04 -4.426564E+01  2.686258E+05  3.974906E+02\n"
                          "    4.00  2.287093E+04 -2.404940E+01  1.989585E+04 -4.781980E+01  2.971014E+05  4.187755E+02\n";
    std::vector<hdb::ListOfMatrixSections> lists_of_sections;
    qi::phrase_parse(s.begin(), s.end(), g.list_of_matrix_sections,blank,lists_of_sections);
    ASSERT_EQ(1,lists_of_sections.size());
    ASSERT_EQ("Added_mass_Radiation_Damping",lists_of_sections.front().header);
    ASSERT_EQ(1,lists_of_sections.front().sections.size());
    ASSERT_EQ("ADDED_MASS_LINE_1",lists_of_sections.front().sections.front().header);
}

TEST_F(HDBParserTest, can_parse_a_list_of_matrix_sections_with_id)
{
    hdb::grammar g;
    const std::string s = " [FROUDE-KRYLOV_FORCES_AND_MOMENTS]\n"
                          " [INCIDENCE_EFM_MOD_001]   0.000000    \n"
                          "    1.00  1.200165E+04  1.893719E+02  2.373585E+03  2.217378E+02  2.450282E+04  1.831288E+03\n"
                          "    2.00  2.231911E+04  1.572288E+02  4.072667E+04  1.841146E+02  3.786741E+05  2.997607E+02\n"
                          "    3.00  4.454653E+04  6.198715E+01  1.529433E+05  1.686518E+02  7.067759E+05  1.722263E+02\n"
                          "    3.50  8.983926E+04  4.694564E+01  8.414533E+04  2.237029E+02  1.951937E+06  1.681252E+02\n"
                          "    3.80  1.582703E+05  3.671505E+01  1.119930E+05  2.245153E+02  2.716110E+06  1.724225E+02\n"
                          "    4.00  1.908930E+05  3.087534E+01  1.897287E+05  2.161797E+02  3.066335E+06  1.666437E+02\n"
                          " [INCIDENCE_EFM_MOD_001]   15.00000    \n"
                          "    1.00  7.876434E+02  5.298045E+02  8.785347E+02  1.825250E+03  9.672805E+03  1.263378E+04\n"
                          "    2.00  1.601279E+04  4.472637E+03  3.173892E+04  3.147891E+04  4.092457E+05  8.631038E+04\n"
                          "    3.00  3.444315E+04  9.285388E+03  1.507413E+05  5.259770E+04  6.919106E+05  1.716833E+05\n"
                          "    3.50  1.020606E+05  2.729635E+04  7.539870E+04  4.399268E+04  2.068251E+06  3.763479E+05\n"
                          "    3.80  1.666161E+05  4.460409E+04  1.309881E+05  5.010179E+04  2.782103E+06  3.926319E+05\n"
                          "    4.00  1.964362E+05  5.260016E+04  2.124103E+05  5.332651E+04  3.099761E+06  3.791878E+05\n";
    hdb::ListOfMatrixSectionsWithId l;
    std::string::const_iterator b = s.begin(), e = s.end();
    const bool res = qi::phrase_parse(b, e, g.list_of_matrix_sections_with_id,blank,l);
    EXPECT_EQ("FROUDE-KRYLOV_FORCES_AND_MOMENTS", l.header);
    EXPECT_EQ(2, l.sections_with_id.size());
    ASSERT_FALSE(l.sections_with_id.empty());

    EXPECT_EQ("INCIDENCE_EFM_MOD_001", l.sections_with_id.at(0).header);
    EXPECT_DOUBLE_EQ(0, l.sections_with_id.at(0).id);
    ASSERT_DOUBLE_EQ(1, l.sections_with_id.at(0).values.at(0).at(0));
    ASSERT_DOUBLE_EQ(1.200165E+04, l.sections_with_id.at(0).values.at(0).at(1));
    ASSERT_DOUBLE_EQ(1.893719E+02, l.sections_with_id.at(0).values.at(0).at(2));
    ASSERT_DOUBLE_EQ(2.373585E+03, l.sections_with_id.at(0).values.at(0).at(3));
    ASSERT_DOUBLE_EQ(2.217378E+02, l.sections_with_id.at(0).values.at(0).at(4));
    ASSERT_DOUBLE_EQ(2.450282E+04, l.sections_with_id.at(0).values.at(0).at(5));
    ASSERT_DOUBLE_EQ(1.831288E+03, l.sections_with_id.at(0).values.at(0).at(6));
    ASSERT_EQ(2,l.sections_with_id.size());
    EXPECT_EQ("INCIDENCE_EFM_MOD_001", l.sections_with_id.at(1).header);
    ASSERT_DOUBLE_EQ(5.260016E+04, l.sections_with_id.at(1).values.at(5).at(2));
    EXPECT_DOUBLE_EQ(15, l.sections_with_id.at(1).id);
}

TEST_F(HDBParserTest, vector_rule_should_not_parse_value)
{
    hdb::grammar g;
    const std::string s = "[FORWARD SPEED] 1.23";
    std::string::const_iterator b = s.begin(), e = s.end();
    EXPECT_FALSE(qi::phrase_parse(b, e, g.vector_section, blank));
    b = s.begin();
    EXPECT_TRUE(qi::phrase_parse(b, e, g.value_key, blank));
}

TEST_F(HDBParserTest, vector_rule_should_not_parse_matrix)
{
    hdb::grammar g;
    const std::string s =   " [Mass_Inertia_matrix]\n"
                            "   2.533000E+05  0.000000E+00  0.000000E+00  0.000000E+00  0.000000E+00  0.000000E+00\n"
                            "   0.000000E+00  2.533000E+05  0.000000E+00  0.000000E+00  0.000000E+00  0.000000E+00\n"
                            "   0.000000E+00  0.000000E+00  2.533000E+05  0.000000E+00  0.000000E+00  0.000000E+00\n"
                            "   0.000000E+00  0.000000E+00  0.000000E+00  1.538000E+06  0.000000E+00  0.000000E+00\n"
                            "   0.000000E+00  0.000000E+00  0.000000E+00  0.000000E+00  8.295000E+06  0.000000E+00\n"
                            "   0.000000E+00  0.000000E+00  0.000000E+00  0.000000E+00  0.000000E+00  8.295000E+06\n";
    EXPECT_FALSE(qi::phrase_parse(s.begin(), s.end(), g.vector_section, blank));
}

TEST_F(HDBParserTest, can_parse_file_to_internal_data_structure)
{
    const hdb::AST hdb_file = hdb::parse(test_data::anthineas_hdb());
    ASSERT_EQ(1, hdb_file.string_keys.size());
    ASSERT_EQ("AQUA+", hdb_file.string_keys.front().value);
    ASSERT_EQ(5, hdb_file.value_keys.size());
    ASSERT_DOUBLE_EQ(0,hdb_file.value_keys.at(0).value);
    ASSERT_DOUBLE_EQ(6,hdb_file.value_keys.at(1).value);
    ASSERT_DOUBLE_EQ(13,hdb_file.value_keys.at(2).value);
    ASSERT_DOUBLE_EQ(0,hdb_file.value_keys.at(3).value);
    ASSERT_DOUBLE_EQ(180,hdb_file.value_keys.at(4).value);
    ASSERT_EQ(1, hdb_file.lists_of_matrix_sections.size());
    ASSERT_EQ(3, hdb_file.lists_of_matrix_sections_with_id.size());
    ASSERT_EQ(2, hdb_file.vector_sections.size());

}
