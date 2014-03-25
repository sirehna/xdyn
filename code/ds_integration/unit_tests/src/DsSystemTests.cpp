/*
 * DsSystemTests.cpp
 *
 *  Created on: 25 mars 2014
 *      Author: maroff
 */

#include "DsSystemTests.hpp"
#include "DsSystemMacros.hpp"
#include "DataSource.hpp"
#include "DataSourceModule.hpp"
#include <Eigen/Dense>

#include "test_macros.hpp"

typedef Eigen::Matrix<double, 6, 1> Force;

DsSystemTests::DsSystemTests() : a(DataGenerator(87514651))
{
}

DsSystemTests::~DsSystemTests()
{
}

void DsSystemTests::SetUp()
{
}

void DsSystemTests::TearDown()
{
}

DEFINE(test, gravity, Force)

FORCE_MODULE(test, ball,           Force g;\
                                   g(3) = -9.81;\
                                   PTR_SET(test::gravity, g))

TEST_F(DsSystemTests, example)
{
//! [DsSystemTests example]
//! [DsSystemTests example]
//! [DsSystemTests expected output]
//! [DsSystemTests expected output]
}
