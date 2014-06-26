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
#include "Kinematics.hpp"
#include "rotation_matrix_builders.hpp"
#include "Transform.hpp"

#define PI (4.*atan(1.))

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
    TR1(shared_ptr)<Kinematics> k(new Kinematics());
    ds.set<TR1(shared_ptr)<Kinematics> >("kinematics", k);
    const RotationMatrix R = kinematics::rot(1,0,0, PI/3.);

    k->add(kinematics::Transform(Point("NED",4,-98,100),R,"foo"));

    ds.set<Wrench>("F1(foo)", Wrench(Point("foo"), Eigen::Vector3d(1, 22, 333), Eigen::Vector3d(-1, -22, -333)));
    ds.set<Wrench>("F2(foo)", Wrench(Point("foo"), Eigen::Vector3d(7, -11, 91), Eigen::Vector3d(-7, 11, -91)));
    ds.set<Eigen::Matrix<double,6,6> >("solid body inertia(foo)", Eigen::Matrix<double,6,6>::Identity());
    ds.set<Eigen::Matrix<double,6,6> >("total inertia(foo)", Eigen::Matrix<double,6,6>());
    ds.set<Eigen::Matrix<double,6,6> >("inverse of the total inertia(foo)", Eigen::Matrix<double,6,6>::Identity());
    ds.set<Point>("G(foo)", Point("foo"));
    ds.set<double>("u(foo)", 79);
    ds.set<double>("v(foo)", 842);
    ds.set<double>("w(foo)", -72);
    ds.set<double>("p(foo)", 116);
    ds.set<double>("q(foo)", 3);
    ds.set<double>("r(foo)", 5);
    ds.set<double>("qr(foo)", 1);
    ds.set<double>("qi(foo)", 2);
    ds.set<double>("qj(foo)", 3);
    ds.set<double>("qk(foo)", 4);
//! [StateDerivativesModule example]
//! [StateDerivativesModule expected output]
    const Wrench F = ds.get<Wrench>("sum of forces(foo)");
    const Wrench Fc = ds.get<Wrench>("coriolis and centripetal forces(foo)");
    ASSERT_DOUBLE_EQ(-4426, Fc.X());
    ASSERT_DOUBLE_EQ(8747, Fc.Y());
    ASSERT_DOUBLE_EQ(97435, Fc.Z());
    ASSERT_DOUBLE_EQ(0, Fc.K());
    ASSERT_DOUBLE_EQ(0, Fc.M());
    ASSERT_DOUBLE_EQ(0, Fc.N());
    ASSERT_EQ("foo", F.get_frame());
    ASSERT_DOUBLE_EQ(8-4426, F.X());
    ASSERT_DOUBLE_EQ(11+8747, F.Y());
    ASSERT_DOUBLE_EQ(424+97435, F.Z());
    ASSERT_DOUBLE_EQ(-8, F.K());
    ASSERT_DOUBLE_EQ(-11, F.M());
    ASSERT_DOUBLE_EQ(-424, F.N());
    ASSERT_DOUBLE_EQ(79, ds.get<double>("dx/dt(foo)"));
    ASSERT_DOUBLE_EQ(358.6461709275205294, ds.get<double>("dy/dt(foo)"));
    ASSERT_DOUBLE_EQ(-765.19338998649732275, ds.get<double>("dz/dt(foo)"));
    ASSERT_DOUBLE_EQ(-4418, ds.get<double>("du/dt(foo)"));
    ASSERT_DOUBLE_EQ(8758, ds.get<double>("dv/dt(foo)"));
    ASSERT_DOUBLE_EQ(97859, ds.get<double>("dw/dt(foo)"));
    ASSERT_DOUBLE_EQ(-8, ds.get<double>("dp/dt(foo)"));
    ASSERT_DOUBLE_EQ(-11, ds.get<double>("dq/dt(foo)"));
    ASSERT_DOUBLE_EQ(-424, ds.get<double>("dr/dt(foo)"));
    ASSERT_DOUBLE_EQ(-130.5, ds.get<double>("dqr/dt(foo)"));
    ASSERT_DOUBLE_EQ(59.5, ds.get<double>("dqi/dt(foo)"));
    ASSERT_DOUBLE_EQ(228.5, ds.get<double>("dqj/dt(foo)"));
    ASSERT_DOUBLE_EQ(-168.5, ds.get<double>("dqk/dt(foo)"));
//! [StateDerivativesModule expected output]
}

