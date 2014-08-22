/*
 * stl_writerTest.cpp
 *
 *  Created on: Aug 22, 2014
 *      Author: cady
 */

#include "stl_writerTest.hpp"
#include "StlReader.hpp"
#include "stl_writer.hpp"
#include "STL_data.hpp"

stl_writerTest::stl_writerTest() : a(DataGenerator(542))
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
