/*
 * HDBBuilderTest.cpp
 *
 *  Created on: Nov 13, 2014
 *      Author: cady
 */

#include "HDBBuilder.hpp"
#include "HDBBuilderTest.hpp"
#include "hdb_data.hpp"

HDBBuilderTest::HDBBuilderTest() : a(ssc::random_data_generator::DataGenerator(833332))
{
}

HDBBuilderTest::~HDBBuilderTest()
{
}

void HDBBuilderTest::SetUp()
{
}

void HDBBuilderTest::TearDown()
{
}

TEST_F(HDBBuilderTest, can_get_added_mass)
{
    const HDBBuilder builder(test_data::anthineas_hdb());
    const auto Ma = builder.get_added_mass();
    ASSERT_EQ(6,Ma.size());
    ASSERT_DOUBLE_EQ(1,Ma.at(0).first);
    ASSERT_EQ(6,Ma.at(0).second.size());
    ASSERT_DOUBLE_EQ(1.097184E+04,Ma.at(0).second.at(0).at(0));
    ASSERT_DOUBLE_EQ(4.443533E+01,Ma.at(0).second.at(3).at(4));

    ASSERT_DOUBLE_EQ(2,Ma.at(1).first);
    ASSERT_EQ(6,Ma.at(1).second.size());
    ASSERT_DOUBLE_EQ(2.102286E+04,Ma.at(1).second.at(1).at(3));
    ASSERT_DOUBLE_EQ(2.702315E+05,Ma.at(1).second.at(2).at(4));

    ASSERT_DOUBLE_EQ(3,Ma.at(2).first);
    ASSERT_EQ(6,Ma.at(2).second.size());
    ASSERT_DOUBLE_EQ(7.183531E+01,Ma.at(2).second.at(5).at(0));
    ASSERT_DOUBLE_EQ(-8.938050E+01,Ma.at(2).second.at(2).at(1));

    ASSERT_DOUBLE_EQ(3.5,Ma.at(3).first);
    ASSERT_EQ(6,Ma.at(3).second.size());

    ASSERT_DOUBLE_EQ(3.8,Ma.at(4).first);
    ASSERT_EQ(6,Ma.at(4).second.size());

    ASSERT_DOUBLE_EQ(4,Ma.at(5).first);
    ASSERT_EQ(6,Ma.at(5).second.size());
}



