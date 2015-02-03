/*
 * RudderForceModelTest.cpp
 *
 *  Created on: Feb 2, 2015
 *      Author: cady
 */

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include "RudderForceModelTest.hpp"
#include "RudderForceModel.hpp"


namespace ssc
{
    namespace random_data_generator
    {
        template <> RudderForceModel::Yaml TypedScalarDataGenerator<RudderForceModel::Yaml>::get() const
        {
            RudderForceModel::Yaml ret;
            ret.Ar = random<double>();
            ret.b = random<double>();
            ret.blade_area_ratio = random<double>();
            ret.diameter = random<double>();
            ret.drag_coeff = random<double>();
            ret.effective_aspect_ratio_factor = random<double>();
            ret.lift_coeff = random<double>();
            ret.nu = random<double>();
            ret.rho = random<double>();
            return ret;
        }
    }
}

RudderForceModelTest::RudderForceModelTest() : a(ssc::random_data_generator::DataGenerator(21212))
{
}

RudderForceModelTest::~RudderForceModelTest()
{
}

void RudderForceModelTest::SetUp()
{
}

void RudderForceModelTest::TearDown()
{
}

TEST_F(RudderForceModelTest, angle_of_attack)
{
//! [RudderForceModelTest get_alpha_example]
    RudderForceModel::RudderModel riw(a.random<RudderForceModel::Yaml>());
//! [RudderForceModelTest get_alpha_example]
//! [RudderForceModelTest get_alpha_example output]
    ASSERT_DOUBLE_EQ(-1,riw.get_angle_of_attack(1, 2));
    ASSERT_DOUBLE_EQ(1,riw.get_angle_of_attack(2, 1));
//! [RudderForceModelTest get_alpha_example output]
}

TEST_F(RudderForceModelTest, get_Cd)
{
    RudderForceModel::Yaml parameters = a.random<RudderForceModel::Yaml>();
    parameters.nu = 2;
    parameters.rho = 1024;
    parameters.Ar = 3;
    parameters.b = 4;
    parameters.effective_aspect_ratio_factor = 0.5;
    RudderForceModel::RudderModel riw(parameters);

    const double Vs = 6;
    const double Cl = 7;
    ASSERT_DOUBLE_EQ(6.5028916553907399, riw.get_Cd(Vs, Cl));
}

TEST_F(RudderForceModelTest, get_Cl)
{
    RudderForceModel::Yaml parameters = a.random<RudderForceModel::Yaml>();
    parameters.nu = 2;
    parameters.rho = 1024;
    parameters.Ar = 3;
    parameters.b = 4;
    parameters.effective_aspect_ratio_factor = 0.5;
    RudderForceModel::RudderModel riw(parameters);
    const double alpha = 0.4;
    ASSERT_DOUBLE_EQ(1.0985577009852809, riw.get_Cl(alpha));
}

TEST_F(RudderForceModelTest, get_lift)
{
    RudderForceModel::Yaml parameters = a.random<RudderForceModel::Yaml>();
    parameters.rho = 1024;
    parameters.Ar = 10;
    parameters.lift_coeff = 2;
    RudderForceModel::RudderModel riw(parameters);

    const double Vs = 12;
    const double Cl = 1.3;
    const double alpha = PI/4;
    const double Ar = 1.4;
    ASSERT_DOUBLE_EQ(189766.19029849127, riw.get_lift(Vs,Cl,alpha,Ar));
}

TEST_F(RudderForceModelTest, get_drag)
{
    RudderForceModel::Yaml parameters = a.random<RudderForceModel::Yaml>();
    parameters.rho = 1024;
    parameters.Ar = 10;
    parameters.drag_coeff = 2;
    RudderForceModel::RudderModel riw(parameters);

    const double Vs = 12;
    const double Cl = 1.3;
    const double alpha = PI/4;
    const double Ar = 1.4;
    ASSERT_DOUBLE_EQ(189766.19029849127, riw.get_drag(Vs,Cl,alpha,Ar));
}

TEST_F(RudderForceModelTest, get_force)
{
//! [RudderForceModelTest get_alpha_example]
    RudderForceModel::RudderModel riw(a.random<RudderForceModel::Yaml>());
    const double drag = 10;
    const double lift = 200;
    const double angle = -PI/3;
    ssc::kinematics::Vector6d v = riw.get_force(drag, lift, angle);
    ASSERT_DOUBLE_EQ(-91.339745962155646, (double)v(0));
    ASSERT_DOUBLE_EQ(178.20508075688772, (double)v(1));
    ASSERT_DOUBLE_EQ(0, (double)v(2));
    ASSERT_DOUBLE_EQ(0, (double)v(3));
    ASSERT_DOUBLE_EQ(0, (double)v(4));
    ASSERT_DOUBLE_EQ(0, (double)v(5));
}

TEST_F(RudderForceModelTest, get_wrench)
{
//! [RudderForceModelTest get_alpha_example]
    RudderForceModel::Yaml parameters;
    parameters.rho = 1024;
    parameters.Ar = 10;
    parameters.drag_coeff = 2;
    parameters.lift_coeff = 5;
    parameters.nu = 0.75;
    parameters.b = 99;
    parameters.effective_aspect_ratio_factor = 2.3;
    RudderForceModel::RudderModel riw(parameters);
    const double drag = 10;
    const double lift = 200;
    const double angle = -PI/3;
    const double area = 1.467;
    ssc::kinematics::Vector6d v = riw.get_wrench(3,4,0.5,area);
    ASSERT_DOUBLE_EQ(-1820.1789942343371, (double)v(0));
    ASSERT_DOUBLE_EQ(1578.008939910043, (double)v(1));
    ASSERT_DOUBLE_EQ(0, (double)v(2));
    ASSERT_DOUBLE_EQ(0, (double)v(3));
    ASSERT_DOUBLE_EQ(0, (double)v(4));
    ASSERT_DOUBLE_EQ(0, (double)v(5));
}

TEST_F(RudderForceModelTest, get_Ar)
{
    RudderForceModel::Yaml parameters = a.random<RudderForceModel::Yaml>();
    RudderForceModel::RudderModel riw(parameters);
    const double CTh = a.random<double>();
    const RudderForceModel::InOutWake ar = riw.get_Ar(CTh);
    ASSERT_DOUBLE_EQ(parameters.Ar, ar.in_wake-ar.outside_wake);
}

TEST_F(RudderForceModelTest, get_Ar2)
{
    RudderForceModel::Yaml parameters = a.random<RudderForceModel::Yaml>();
    parameters.Ar = 10;
    parameters.b = 4;
    parameters.distance_between_rudder_and_screw = 2.47;
    parameters.diameter = 1.67;
    const double CTh = 5.3;
    RudderForceModel::RudderModel riw(parameters);
    const auto ar = riw.get_Ar(CTh);
    ASSERT_DOUBLE_EQ(3.5404447215261827, ar.in_wake);
    ASSERT_DOUBLE_EQ(-6.4595552784738173, ar.outside_wake);
}

TEST_F(RudderForceModelTest, get_Vs)
{
    RudderForceModel::Yaml parameters = a.random<RudderForceModel::Yaml>();
    parameters.diameter = 3.6;
    parameters.rho = 1024;
    RudderForceModel::RudderModel riw(parameters);
    const RudderForceModel::InOutWake vs = riw.get_vs(1.5,12,6,12e4);
    ASSERT_DOUBLE_EQ(8.6501382915679557, vs.in_wake);
    ASSERT_DOUBLE_EQ(13.416407864998739, vs.outside_wake);
}
