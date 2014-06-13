/*
 * DsSystemTests.cpp
 *
 *  Created on: 25 mars 2014
 *      Author: cec
 */

#include "DsSystemTests.hpp"
#include "DsSystemMacros.hpp"
#include "DsSystem.hpp"
#include "DsSolve.hpp"
#include "DsCsvObserver.hpp"
#include "test_macros.hpp"


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

DEFINE(falling_ball, z, double)
DEFINE(falling_ball, zdot, double)
DEFINE(falling_ball, zdotdot, double)

TEST_F(DsSystemTests, example)
{
//! [DsSystemTests example]
    DataSource ds;
    SET(ds, falling_ball::zdotdot, -9.81);
    SET(ds, falling_ball::zdot, 0);
    SET(ds, falling_ball::z, 0);
    SET(ds, simulator_base::initial_time_step, 1);
    SET(ds, simulator_base::stepper, solver::EULER);
    DEFINE_DERIVATIVE(falling_ball::z, falling_ball::zdot, ds);
    DEFINE_DERIVATIVE(falling_ball::zdot, falling_ball::zdotdot, ds);
    DsCsvObserver observer(std::cout);

    integrate(ds, 0, 10, observer);
//! [DsSystemTests example]
//! [DsSystemTests expected output]
//! [DsSystemTests expected output]
}

TEST_F(DsSystemTests, throws_if_initial_time_step_is_not_defined)
{
    DataSource ds;
    SET(ds, falling_ball::zdotdot, -9.81);
    SET(ds, falling_ball::zdot, 0);
    SET(ds, falling_ball::z, 0);
    SET(ds, simulator_base::stepper, solver::EULER);
    DEFINE_DERIVATIVE(falling_ball::z, falling_ball::zdot, ds);
    DEFINE_DERIVATIVE(falling_ball::zdot, falling_ball::zdotdot, ds);
    DsCsvObserver observer(std::cout);

    ASSERT_THROW(integrate(ds, 0, 10, observer),DataSourceException);
}

TEST_F(DsSystemTests, throws_if_stepper_is_not_defined)
{
    DataSource ds;
    SET(ds, falling_ball::zdotdot, -9.81);
    SET(ds, falling_ball::zdot, 0);
    SET(ds, falling_ball::z, 0);
    SET(ds, simulator_base::initial_time_step, 1);
    DEFINE_DERIVATIVE(falling_ball::z, falling_ball::zdot, ds);
    DEFINE_DERIVATIVE(falling_ball::zdot, falling_ball::zdotdot, ds);
    DsCsvObserver observer(std::cout);

    ASSERT_THROW(integrate(ds, 0, 10, observer),DataSourceException);
}
