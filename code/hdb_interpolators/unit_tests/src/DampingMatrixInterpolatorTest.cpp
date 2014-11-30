/*
 * DampingMatrixInterpolatorTest.cpp
 *
 *  Created on: Nov 27, 2014
 *      Author: cady
 */


#include "DampingMatrixInterpolatorTest.hpp"
#include "DampingMatrixInterpolator.hpp"

#include <ssc/macros.hpp>

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

DampingMatrixInterpolatorTest::DampingMatrixInterpolatorTest() : a(ssc::random_data_generator::DataGenerator(212))
{
}

DampingMatrixInterpolatorTest::~DampingMatrixInterpolatorTest()
{
}

void DampingMatrixInterpolatorTest::SetUp()
{
}

void DampingMatrixInterpolatorTest::TearDown()
{
}
double f(const double omega);
double f(const double omega)
{
    return 0.5*(0.1/(0.01+(0.5-omega)*(0.5-omega))+0.1/(0.01+(0.5+omega)*(0.5+omega)));
}

TEST_F(DampingMatrixInterpolatorTest, all_types_of_interpolator_can_retrieve_original_values)
{
    std::vector<double> omega;
    std::vector<double> B;
    const double omega_min = 0;
    const double omega_max = 4;
    size_t N = 100;
    for (size_t i = 0 ; i < N ; ++i)
    {
        omega.push_back(omega_min + (omega_max-omega_min)*((double)i)/((double)(N-1)));
        B.push_back(f(omega.back()));
    }

    DampingMatrixInterpolator factory;
    auto f1 = factory.build_interpolator(omega, B, TypeOfInterpolation::LINEAR);
    auto f2 = factory.build_interpolator(omega, B, TypeOfInterpolation::PIECEWISE_CONSTANT);
    auto f3 = factory.build_interpolator(omega, B, TypeOfInterpolation::SPLINES);

    for (size_t i = 0 ; i < N ; ++i)
    {
        ASSERT_NEAR(B.at(i), f1(omega.at(i)), 1E-10) << "i = " << i;
        ASSERT_NEAR(B.at(i), f2(omega.at(i)), 1E-10) << "i = " << i;
        ASSERT_NEAR(B.at(i), f3(omega.at(i)), 1E-10) << "i = " << i;
    }
}

TEST_F(DampingMatrixInterpolatorTest, can_calculate_cosine_transform)
{
    const auto B = [](const double ){return 1;};
    DampingMatrixInterpolator factory;
    const double omega_min = 2*PI/10;
    const double omega_max = 2*PI/1;
    const size_t n = 10;
    const auto K = factory.make_retardation_function(B, omega_min, omega_max, n, TypeOfInterpolation::SPLINES, TypeOfQuadrature::GAUSS_KRONROD);
    double tau = 3;
    ASSERT_NEAR(2./PI*(sin(omega_max*tau)/tau-sin(omega_min*tau)/tau), K(tau), 1E-10);
}
