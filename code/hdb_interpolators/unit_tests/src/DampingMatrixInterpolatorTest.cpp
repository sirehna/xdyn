/*
 * DampingMatrixInterpolatorTest.cpp
 *
 *  Created on: Nov 27, 2014
 *      Author: cady
 */


#include "DampingMatrixInterpolatorTest.hpp"
#include "DampingMatrixInterpolator.hpp"

#include <ssc/macros.hpp>

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
        ASSERT_NEAR(B.at(i), f1->f(omega.at(i)), 1E-10) << "i = " << i;
        ASSERT_NEAR(B.at(i), f2->f(omega.at(i)), 1E-10) << "i = " << i;
        ASSERT_NEAR(B.at(i), f3->f(omega.at(i)), 1E-10) << "i = " << i;
    }
}

