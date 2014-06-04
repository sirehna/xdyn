/*
 * StateDerivativesModule.cpp
 *
 *  Created on: May 29, 2014
 *      Author: cady
 */

#include "StateDerivativesModuleTest.hpp"
#include "StateDerivativesModule.hpp"
#include "DataSource.hpp"
#include "Wrench.hpp"

StateDerivativesModuleTest::StateDerivativesModuleTest() : a(DataGenerator(9898))
{
}

StateDerivativesModuleTest::~StateDerivativesModuleTest()
{
}

void StateDerivativesModuleTest::SetUp()
{
}

void StateDerivativesModuleTest::TearDown()
{
}

TEST_F(StateDerivativesModuleTest, example)
{
//! [StateDerivativesModule example]
    const std::vector<std::string> forces = {"F1", "F2"};
    const std::string body = "foo";
    DataSource ds;
    const StateDerivativesModule module(&ds, body, forces);
    ds.add(module);
    ds.set<Wrench>("F1(foo)", Wrench(Point("foo"), Eigen::Vector3d(1, 22, 333), Eigen::Vector3d(-1, -22, -333)));
    ds.set<Wrench>("F2(foo)", Wrench(Point("foo"), Eigen::Vector3d(7, -11, 91), Eigen::Vector3d(-7, 11, -91)));
    ds.set<Eigen::Matrix<double,6,6> >("solid body inertia(foo)", Eigen::Matrix<double,6,6>::Identity());
    ds.set<Eigen::Matrix<double,6,6> >("total inertia(foo)", Eigen::Matrix<double,6,6>());
    ds.set<Eigen::Matrix<double,6,6> >("inverse of the total inertia(foo)", Eigen::Matrix<double,6,6>::Identity());
    ds.set<Point>("G(foo)", Point("foo"));
    ds.set<double>("u(foo)", 0);
    ds.set<double>("v(foo)", 0);
    ds.set<double>("w(foo)", 0);
    ds.set<double>("p(foo)", 0);
    ds.set<double>("q(foo)", 0);
    ds.set<double>("r(foo)", 0);
//! [StateDerivativesModule example]
//! [StateDerivativesModule expected output]
    const Wrench F = ds.get<Wrench>("sum of forces(foo)");
    ASSERT_EQ("foo", F.get_frame());
    ASSERT_DOUBLE_EQ(8, F.X);
    ASSERT_DOUBLE_EQ(11, F.Y);
    ASSERT_DOUBLE_EQ(424, F.Z);
    ASSERT_DOUBLE_EQ(-8, F.K);
    ASSERT_DOUBLE_EQ(-11, F.M);
    ASSERT_DOUBLE_EQ(-424, F.N);
//! [StateDerivativesModule expected output]
}

