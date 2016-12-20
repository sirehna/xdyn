/*
 * StretchingTest.cpp
 *
 *  Created on: Dec 20, 2016
 *      Author: cady
 */

#include "StretchingTest.hpp"
#include "Stretching.hpp"
#include "YamlWaveModelInput.hpp"

StretchingTest::StretchingTest() : a(ssc::random_data_generator::DataGenerator(88223232))
{
}

StretchingTest::~StretchingTest()
{
}

void StretchingTest::SetUp()
{
}

void StretchingTest::TearDown()
{
}

TEST_F(StretchingTest, example)
{
    //! [StretchingTest example]
    YamlStretching yaml;
    yaml.delta = 0.4;
    yaml.h = 123;
    const Stretching stretching(yaml);
    //! [StretchingTest example]
    //! [StretchingTest expected output]
    ASSERT_DOUBLE_EQ(124, stretching.rescaled_z(124, -10));
    ASSERT_DOUBLE_EQ(124, stretching.rescaled_z(124, 10));
    ASSERT_DOUBLE_EQ(124, stretching.rescaled_z(124, 0));
    ASSERT_DOUBLE_EQ(125, stretching.rescaled_z(125, -10));
    ASSERT_DOUBLE_EQ(125, stretching.rescaled_z(125, 10));
    ASSERT_DOUBLE_EQ(125, stretching.rescaled_z(125, 0));
    ASSERT_DOUBLE_EQ(123, stretching.rescaled_z(123, -10));
    ASSERT_DOUBLE_EQ(123, stretching.rescaled_z(123, 10));
    ASSERT_DOUBLE_EQ(123, stretching.rescaled_z(123, 0));
    ASSERT_DOUBLE_EQ(122.04511278195488, stretching.rescaled_z(122, -10));
    ASSERT_DOUBLE_EQ(121.94690265486726, stretching.rescaled_z(122, 10));
    ASSERT_DOUBLE_EQ(122, stretching.rescaled_z(122, 0));
    ASSERT_DOUBLE_EQ(5.5488721804511272, stretching.rescaled_z(0, -10));
    ASSERT_DOUBLE_EQ(-6.5309734513274407, stretching.rescaled_z(0, 10));
    ASSERT_DOUBLE_EQ(0, stretching.rescaled_z(0, 0));
    ASSERT_DOUBLE_EQ(-89.939849624060145, stretching.rescaled_z(-100, -10));
    ASSERT_DOUBLE_EQ(-111.84070796460176, stretching.rescaled_z(-100, 10));
    ASSERT_DOUBLE_EQ(-100, stretching.rescaled_z(-100, 0));
    //! [StretchingTest expected output]
}
