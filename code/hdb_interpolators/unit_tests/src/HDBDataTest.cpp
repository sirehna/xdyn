/*
 * HDBDataTest.cpp
 *
 *  Created on: Nov 13, 2014
 *      Author: cady
 */

#include "HDBBuilder.hpp"
#include "HDBData.hpp"
#include "HDBDataTest.hpp"
#include "hdb_data.hpp"

#include <ssc/macros.hpp>

HDBDataTest::HDBDataTest() : a(ssc::random_data_generator::DataGenerator(822243))
{
}

HDBDataTest::~HDBDataTest()
{
}

void HDBDataTest::SetUp()
{
}

void HDBDataTest::TearDown()
{
}

TEST_F(HDBDataTest, can_retrieve_initial_values)
{
//! [HDBDataTest example]
    HDBData data((HDBBuilder(test_data::anthineas_hdb())));
    //! [HDBDataTest example]

    //! [HDBDataTest expected output]
    const auto M1 = data.get_added_mass(1);
    ASSERT_EQ(6, M1.cols());
    ASSERT_EQ(6, M1.rows());
    ASSERT_DOUBLE_EQ(1.503212E+04, (double)M1(2,0));
    ASSERT_DOUBLE_EQ(-1.618101E+02, (double)M1(2,1));
    ASSERT_DOUBLE_EQ(1.781351E+05, (double)M1(2,2));
    ASSERT_DOUBLE_EQ(-1.053934E+02, (double)M1(2,3));
    ASSERT_DOUBLE_EQ(3.267841E+05, (double)M1(2,4));
    ASSERT_DOUBLE_EQ(6.774041E+02, (double)M1(2,5));

    const auto M2 = data.get_added_mass(2);
    ASSERT_EQ(6, M2.cols());
    ASSERT_EQ(6, M2.rows());
    ASSERT_DOUBLE_EQ(3.032333E+01, (double)M2(3,0));
    ASSERT_DOUBLE_EQ(2.332197E+04, (double)M2(3,1));
    ASSERT_DOUBLE_EQ(-5.924372E+01, (double)M2(3,2));
    ASSERT_DOUBLE_EQ(2.132689E+05, (double)M2(3,3));
    ASSERT_DOUBLE_EQ(2.994200E+02, (double)M2(3,4));
    ASSERT_DOUBLE_EQ(3.943479E+04, (double)M2(3,5));
//! [HDBDataTest expected output]
}

TEST_F(HDBDataTest, can_retrieve_added_mass_at_Tp_0)
{
    HDBData data((HDBBuilder(test_data::anthineas_hdb())));
    const auto M = data.get_added_mass();
    ASSERT_EQ(6, M.cols());
    ASSERT_EQ(6, M.rows());
    ASSERT_DOUBLE_EQ(1.503212E+04, (double)M(2,0));
    ASSERT_DOUBLE_EQ(-1.618101E+02, (double)M(2,1));
    ASSERT_DOUBLE_EQ(1.781351E+05, (double)M(2,2));
    ASSERT_DOUBLE_EQ(-1.053934E+02, (double)M(2,3));
    ASSERT_DOUBLE_EQ(3.267841E+05, (double)M(2,4));
    ASSERT_DOUBLE_EQ(6.774041E+02, (double)M(2,5));
}

TEST_F(HDBDataTest, can_retrieve_periods_for_radiation_damping)
{
    HDBData data((HDBBuilder(test_data::anthineas_hdb())));
    const auto periods = data.get_radiation_damping_periods();
    ASSERT_EQ(6, periods.size());
    ASSERT_EQ(1,   periods.at(0));
    ASSERT_EQ(2,   periods.at(1));
    ASSERT_EQ(3,   periods.at(2));
    ASSERT_EQ(3.5, periods.at(3));
    ASSERT_EQ(3.8, periods.at(4));
    ASSERT_EQ(4,   periods.at(5));
}

TEST_F(HDBDataTest, can_retrieve_vectors_for_each_element_in_radiation_damping_matrix)
{
    HDBData data((HDBBuilder(test_data::anthineas_hdb())));
    for (size_t i = 0 ; i < 6 ; ++i)
    {
        for (size_t j = 0 ; j < 6 ; ++j)
        {
            ASSERT_EQ(6, data.get_radiation_damping_coeff(i,j).size());
        }
    }
    const auto v = data.get_radiation_damping_coeff(1,2);
    ASSERT_DOUBLE_EQ(-1.590935E+02, v.at(0));
    ASSERT_DOUBLE_EQ( 2.595528E+02, v.at(1));
    ASSERT_DOUBLE_EQ(-1.614637E+02, v.at(2));
    ASSERT_DOUBLE_EQ(-1.376756E+02, v.at(3));
    ASSERT_DOUBLE_EQ(-1.215545E+02, v.at(4));
    ASSERT_DOUBLE_EQ(-1.083372E+02, v.at(5));
}
