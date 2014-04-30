/*
 * hydrostaticTest.cpp
 *
 *  Created on: Apr 30, 2014
 *      Author: cady
 */

#include "hydrostaticTest.hpp"
#include "hydrostatic.hpp"

hydrostaticTest::hydrostaticTest() : a(DataGenerator(2))
{
}

hydrostaticTest::~hydrostaticTest()
{
}

void hydrostaticTest::SetUp()
{
}

void hydrostaticTest::TearDown()
{
}

using namespace hydrostatic;

TEST_F(hydrostaticTest, should_be_able_to_compute_type_of_immersion)
{
//! [hydrostaticTest type_of_immersion]
    size_t index[3]; index[0] = 0; index[1] = 1; index[2] = 2;
    const std::vector<double> v({-1,2,3});
//! [hydrostaticTest type_of_immersion]
//! [hydrostaticTest type_of_immersion output]
    ASSERT_FALSE(totally_immerged(index, v));
//! [hydrostaticTest type_of_immersion output]
}



