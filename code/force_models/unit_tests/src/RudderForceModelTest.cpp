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
    RudderForceModel::InWake riw(a.random<RudderForceModel::Yaml>());
    RudderForceModel::OutsideWake row(a.random<RudderForceModel::Yaml>());
//! [RudderForceModelTest get_alpha_example]
//! [RudderForceModelTest get_alpha_example output]
    for (size_t i = 0 ; i < 100 ; ++i)
    {
        const double beta = a.random<double>();
        const double alpha = a.random<double>();
        ASSERT_DOUBLE_EQ(riw.get_angle_of_attack(beta, alpha),row.get_angle_of_attack(beta, alpha));
        ASSERT_DOUBLE_EQ(0,riw.get_angle_of_attack(alpha, alpha));
    }
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
    RudderForceModel::InWake riw(parameters);
    RudderForceModel::OutsideWake row(parameters);
    for (size_t i = 0 ; i < 100 ; ++i)
    {
        const double Vs = a.random<double>().between(0,30);
        const double Cl = a.random<double>().between(-1,1);
        ASSERT_DOUBLE_EQ(riw.get_Cd(Vs, Cl),row.get_Cd(Vs, Cl));
    }
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
    RudderForceModel::InWake riw(parameters);
    RudderForceModel::OutsideWake row(parameters);
    for (size_t i = 0 ; i < 100 ; ++i)
    {
        const double alpha = a.random<double>().between(-PI,PI);
        ASSERT_DOUBLE_EQ(riw.get_Cl(alpha),row.get_Cl(alpha));
    }
    const double alpha = 0.4;
    ASSERT_DOUBLE_EQ(1.0985577009852809, riw.get_Cl(alpha));
}

TEST_F(RudderForceModelTest, get_lift)
{
    RudderForceModel::Yaml parameters = a.random<RudderForceModel::Yaml>();
    parameters.rho = 1024;
    parameters.Ar = 10;
    parameters.lift_coeff = 2;
    RudderForceModel::InWake riw(parameters);
    RudderForceModel::OutsideWake row(parameters);

    for (size_t i = 0 ; i < 100 ; ++i)
    {
        const double Vs = a.random<double>().between(0,50);
        const double Cl = a.random<double>().between(0,2);
        const double alpha = a.random<double>().between(-PI,PI);
        ASSERT_DOUBLE_EQ(riw.get_lift(Vs,Cl,alpha),row.get_lift(Vs,Cl,alpha));
    }
    const double Vs = 12;
    const double Cl = 1.3;
    const double alpha = PI/4;
    ASSERT_DOUBLE_EQ(2*677736.39392318309, riw.get_lift(Vs,Cl,alpha));
}

TEST_F(RudderForceModelTest, get_drag)
{
    RudderForceModel::Yaml parameters = a.random<RudderForceModel::Yaml>();
    parameters.rho = 1024;
    parameters.Ar = 10;
    parameters.drag_coeff = 2;
    RudderForceModel::InWake riw(parameters);
    RudderForceModel::OutsideWake row(parameters);

    for (size_t i = 0 ; i < 100 ; ++i)
    {
        const double Vs = a.random<double>().between(0,50);
        const double Cl = a.random<double>().between(0,2);
        const double alpha = a.random<double>().between(-PI,PI);
        ASSERT_DOUBLE_EQ(riw.get_drag(Vs,Cl,alpha),row.get_drag(Vs,Cl,alpha));
    }
    const double Vs = 12;
    const double Cl = 1.3;
    const double alpha = PI/4;
    ASSERT_DOUBLE_EQ(2*677736.39392318309, riw.get_drag(Vs,Cl,alpha));
}

TEST_F(RudderForceModelTest, get_wake_angle_outside_wake)
{
    RudderForceModel::OutsideWake row(a.random<RudderForceModel::Yaml>());
    ASSERT_DOUBLE_EQ(PI/4, row.get_wake_angle(1,1));
    ASSERT_DOUBLE_EQ(3.*PI/4., row.get_wake_angle(-1,1));
}
