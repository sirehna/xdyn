/*
 * DampingMatrixInterpolatorTest.cpp
 *
 *  Created on: Nov 27, 2014
 *      Author: cady
 */


#include "History.hpp"
#include "RadiationDampingBuilderTest.hpp"
#include "RadiationDampingBuilder.hpp"

#include <ssc/integrate.hpp>
#include <ssc/macros.hpp>

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#define EPS (1E-10)

RadiationDampingBuilderTest::RadiationDampingBuilderTest() : a(ssc::random_data_generator::DataGenerator(212))
{
}

RadiationDampingBuilderTest::~RadiationDampingBuilderTest()
{
}

void RadiationDampingBuilderTest::SetUp()
{
}

void RadiationDampingBuilderTest::TearDown()
{
}
double f(const double omega);
double f(const double omega)
{
    return 0.5*(0.1/(0.01+(0.5-omega)*(0.5-omega))+0.1/(0.01+(0.5+omega)*(0.5+omega)));
}

TEST_F(RadiationDampingBuilderTest, all_types_of_interpolator_can_retrieve_original_values)
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

    RadiationDampingBuilder builder1(TypeOfInterpolation::LINEAR, TypeOfQuadrature::GAUSS_KRONROD);
    RadiationDampingBuilder builder2(TypeOfInterpolation::PIECEWISE_CONSTANT, TypeOfQuadrature::GAUSS_KRONROD);
    RadiationDampingBuilder builder3(TypeOfInterpolation::SPLINES, TypeOfQuadrature::GAUSS_KRONROD);
    auto f1 = builder1.build_interpolator(omega, B);
    auto f2 = builder2.build_interpolator(omega, B);
    auto f3 = builder3.build_interpolator(omega, B);

    for (size_t i = 0 ; i < N ; ++i)
    {
        ASSERT_NEAR(B.at(i), f1(omega.at(i)), EPS) << "i = " << i;
        ASSERT_NEAR(B.at(i), f2(omega.at(i)), EPS) << "i = " << i;
        ASSERT_NEAR(B.at(i), f3(omega.at(i)), EPS) << "i = " << i;
    }
}

TEST_F(RadiationDampingBuilderTest, can_calculate_cosine_transform)
{
    const auto B = [](const double ){return 1;};
    RadiationDampingBuilder builder(TypeOfInterpolation::SPLINES, TypeOfQuadrature::GAUSS_KRONROD);
    const double omega_min = 2*PI/10;
    const double omega_max = 2*PI/1;
    const size_t n = 10;
    std::vector<double> taus;
    for (size_t i = 0 ; i < n ; ++i) taus.push_back(1+9*(double)i/((double)(n-1)));
    const auto K = builder.build_retardation_function(B, taus);
    double tau = 3;
    ASSERT_NEAR(2./PI*(sin(omega_max*tau)/tau-sin(omega_min*tau)/tau), K(tau), EPS);
}

TEST_F(RadiationDampingBuilderTest, can_compute_convolution)
{
    History h(1000);
    h.record(0,1);
    h.record(1000,1);
    RadiationDampingBuilder builder(TypeOfInterpolation::SPLINES, TypeOfQuadrature::GAUSS_KRONROD);
    ASSERT_NEAR(sin(2000.)/2., builder.convolution(h, [](const double t){return cos(2*t);}, 0, 1000), EPS);
}

TEST_F(RadiationDampingBuilderTest, retardation_function_is_correct)
{
    const auto Br = [](const double omega){return 0.5*(0.1/(0.01+(0.5-omega)*(0.5-omega))+0.1/(0.01+(0.5+omega)*(0.5+omega)));};
    RadiationDampingBuilder builder(TypeOfInterpolation::SPLINES, TypeOfQuadrature::SIMPSON);
    const double omega_min = 0.01;//2*PI/10;
    const double omega_max = 850;//00;//2*PI/1;
    const size_t n = 10;
    std::vector<double> taus;
    for (size_t i = 0 ; i < n ; ++i) taus.push_back(2*PI/omega_max+2*PI*(1./omega_min-1./omega_max)*(double)i/((double)(n-1)));
    const auto K = builder.build_retardation_function(Br, taus);
    size_t N = n;
    const double tau_min = 2*PI/omega_max;
    const double tau_max = 2*PI/omega_min;
    for (size_t i = 0 ; i < N ; ++i)
    {
        const double tau = tau_min + (tau_max-tau_min)*double(i)/double(N-1);
        const double K_analytical = 2./PI*ssc::integrate::Simpson([tau,Br](const double t){return Br(t)*cos(tau*t);}).integrate_f(omega_min, omega_max);
        ASSERT_NEAR(K_analytical, K(tau), EPS) << "for tau = " << tau;
    }
}
