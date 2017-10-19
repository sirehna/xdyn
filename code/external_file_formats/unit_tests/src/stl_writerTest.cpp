/*
 * stl_writerTest.cpp
 *
 *  Created on: Aug 22, 2014
 *      Author: cady
 */

#include "stl_writerTest.hpp"
#include "stl_reader.hpp"
#include "stl_writer.hpp"
#include "stl_data.hpp"
#include "base91.hpp"
#include "TriMeshTestData.hpp"

stl_writerTest::stl_writerTest() : a(ssc::random_data_generator::DataGenerator(542))
{
}

stl_writerTest::~stl_writerTest()
{
}

void stl_writerTest::SetUp()
{
}

void stl_writerTest::TearDown()
{
}

TEST_F(stl_writerTest, example)
{
    const VectorOfVectorOfPoints expected = read_stl(test_data::three_facets());
    const VectorOfVectorOfPoints actual = read_stl(write_stl(expected));
    ASSERT_EQ(expected.size(), actual.size());
    for (size_t i = 0 ; i < expected.size() ; ++i)
    {
        ASSERT_EQ(expected[i].size(), actual[i].size());
        for (size_t j = 0 ; j < expected[i].size() ; ++j)
        {
            ASSERT_DOUBLE_EQ((double)expected[i][j](0), (double)actual[i][j](0));
            ASSERT_DOUBLE_EQ((double)expected[i][j](1), (double)actual[i][j](1));
            ASSERT_DOUBLE_EQ((double)expected[i][j](2), (double)actual[i][j](2));
        }
    }
}

TEST_F(stl_writerTest, can_read_and_write_binary_stl)
{
    const VectorOfVectorOfPoints mesh = unit_cube();
    std::stringstream ss;

    write_binary_stl(mesh, ss);
    const VectorOfVectorOfPoints parsed_mesh = read_binary_stl(ss);

    ASSERT_EQ(mesh.size(), parsed_mesh.size());
    for (size_t i = 0 ; i < mesh.size() ; ++i)
    {
        ASSERT_EQ(mesh[i].size(), parsed_mesh[i].size()) << " (for i = " << i << ")";
        for (size_t j = 0 ; j < mesh[i].size() ; ++j)
        {
            ASSERT_DOUBLE_EQ((double)mesh[i][j](0), (double)parsed_mesh[i][j](0)) << " (for i = " << i << " and j = " << j << ", x-coordinate)";
            ASSERT_DOUBLE_EQ((double)mesh[i][j](1), (double)parsed_mesh[i][j](1)) << " (for i = " << i << " and j = " << j << ", y-coordinate)";
            ASSERT_DOUBLE_EQ((double)mesh[i][j](2), (double)parsed_mesh[i][j](2)) << " (for i = " << i << " and j = " << j << ", z-coordinate)";
        }
    }
}

TEST_F(stl_writerTest, can_read_binary_stl_from_string)
{
    const VectorOfVectorOfPoints from_ascii = read_stl(test_data::cube());
    const std::string s = ";C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$AMAAAAAAAAAAAAAAA4}BAC\"EBAA1hAAT|DAAA#AAA1hAAT|DAAA#AAAHLAAT|DAAAAAAAAAAAAAk_HAAASEAAcsBA+>PAAA+CAA4}AAv(fAAA.QAAT|BABt#AAAAAAAAAAAAAC\"JCAAOWAA+>XAAAAAAAP(AA+>XAAAAAAAP(AA+>HAAAAAAAAAAAAAAAAAAATEAAcsAAv(vAAAAAAAcsBAv(PAAAAAAAcsAAv(PAAAAAAAAA4}FAAAAAAAAAAA4}CABtEBAAAAAAT|FAC\"EBAAPLAAk_LAAA#AAAPLAAAA+>vAAAAAAAAAAA+>XAAASEAAAAAA+>XAAAjBAA8sAAv(vAAA+CAAAAAAAABt#AAAAAAAAAAABtfAAA.QAA4}BAC\"fAAA.QAAAAAABtfAAAxFAAAAAAAAC\"#AAAuWAAAAAAC\"fAAA.QAA4}BAC\"fAAAxFAAAAAABtfAAAxFAA4}BAAAAAAAAAf(AAAAAAAAJCAAP(AAAAAAAA#AAA1hAAAAAAC\"fAAA.QAA4}BAAAAAAAAAf(AAAAAAAAJCAAP(AAAAAAAA#AAA1hAAT|DAAAJCAAP(AAk_HAAAAAXLAA4}BAAAAAAA.QAAT|BABt#AAAHLAAk_DAC\"#AAAHLAAk_DAAAAAAAAAAAAAk_HAAAAAAAP(AA+>HAAAkBAAcsAAv(PAAAAAAAcsBAv(PAAAAAAA";
    const VectorOfVectorOfPoints from_binary = read_binary_stl(base<91>::decode(s));

    ASSERT_EQ(from_ascii.size(),from_binary.size());
    for (size_t i = 0 ; i < from_ascii.size() ; ++i)
    {
        ASSERT_EQ(from_ascii[i].size(),from_binary[i].size());
        for (size_t j = 0 ; j < from_ascii[i].size() ; ++j)
        {
            ASSERT_DOUBLE_EQ((double)from_ascii[i][j](0),(double)from_binary[i][j](0));
            ASSERT_DOUBLE_EQ((double)from_ascii[i][j](1),(double)from_binary[i][j](1));
            ASSERT_DOUBLE_EQ((double)from_ascii[i][j](2),(double)from_binary[i][j](2));
        }
    }
}
