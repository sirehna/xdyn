/*
 * DampingMatrixInterpolatorTest.cpp
 *
 *  Created on: Nov 27, 2014
 *      Author: cady
 */

#include "hdb_data.hpp"
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
        B.push_back(test_data::analytical_Br(omega.back()));
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
    std::vector<double> taus = builder.build_regular_intervals(1, 10, n);
    const auto K = builder.build_retardation_function(B, taus, 0);
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
    RadiationDampingBuilder builder(TypeOfInterpolation::SPLINES, TypeOfQuadrature::SIMPSON);
    const double omega_min = 0.01;//2*PI/10;
    const double omega_max = 850;//00;//2*PI/1;
    const size_t n = 10;
    std::vector<double> taus;
    for (size_t i = 0 ; i < n ; ++i) taus.push_back(2*PI/omega_max+2*PI*(1./omega_min-1./omega_max)*(double)i/((double)(n-1)));
    const auto K = builder.build_retardation_function(test_data::analytical_Br, taus, 0);
    size_t N = n;
    const double tau_min = 2*PI/omega_max;
    const double tau_max = 2*PI/omega_min;
    for (size_t i = 0 ; i < N ; ++i)
    {
        const double tau = tau_min + (tau_max-tau_min)*double(i)/double(N-1);
        const double K_analytical = 2./PI*ssc::integrate::Simpson([tau](const double t){return test_data::analytical_Br(t)*cos(tau*t);}).integrate_f(omega_min, omega_max);
        ASSERT_NEAR(K_analytical, K(tau), EPS) << "for tau = " << tau;
    }
}

TEST_F(RadiationDampingBuilderTest, can_create_linearly_spaced_intervals)
{
    RadiationDampingBuilder builder(TypeOfInterpolation::SPLINES, TypeOfQuadrature::SIMPSON);
    for (size_t k = 0 ; k < 10 ; ++k)
    {
        const double xmin = a.random<double>();
        const double xmax = a.random<double>().greater_than(xmin);
        const size_t n = a.random<size_t>().between(3,200);
        const auto v = builder.build_regular_intervals(xmin, xmax, n);
        ASSERT_EQ(n, v.size());
        ASSERT_SMALL_RELATIVE_ERROR(xmin, v.front(),EPS);
        ASSERT_SMALL_RELATIVE_ERROR(xmax, v.back(),EPS);
        const double delta = v[1]-v[0];
        for (size_t i = 2 ; i < n ; ++i)
        {
            ASSERT_SMALL_RELATIVE_ERROR(delta, v[i]-v[i-1],EPS);
        }
    }
}

TEST_F(RadiationDampingBuilderTest, can_create_exponentially_spaced_intervals)
{
    RadiationDampingBuilder builder(TypeOfInterpolation::SPLINES, TypeOfQuadrature::SIMPSON);
    for (size_t k = 0 ; k < 10 ; ++k)
    {
        const double xmin = a.random<double>();
        const double xmax = a.random<double>().greater_than(xmin);
        const size_t n = a.random<size_t>().between(3,200);
        const auto v = builder.build_exponential_intervals(xmin, xmax, n);
        ASSERT_EQ(n, v.size());
        ASSERT_SMALL_RELATIVE_ERROR(xmin, v.front(),EPS);
        ASSERT_SMALL_RELATIVE_ERROR(xmax, v.back(),EPS);
        double delta = v[1]-v[0];
        for (size_t i = 2 ; i < n ; ++i)
        {
            ASSERT_LT(std::abs(delta), std::abs(v[i]-v[i-1]));
            delta = v[i]-v[i-1];
        }
    }
}

TEST_F(RadiationDampingBuilderTest, build_exponential_intervals_works_with_negative_values)
{
    RadiationDampingBuilder builder(TypeOfInterpolation::SPLINES, TypeOfQuadrature::SIMPSON);
    for (size_t k = 0 ; k < 10 ; ++k)
    {
        const double xmin = a.random<double>().no().greater_than(0);
        const double xmax = a.random<double>().between(xmin,0);
        const size_t n = a.random<size_t>().between(3,200);
        const auto v = builder.build_exponential_intervals(xmin, xmax, n);
        ASSERT_EQ(n, v.size());
        ASSERT_SMALL_RELATIVE_ERROR(xmin, v.front(),EPS);
        ASSERT_SMALL_RELATIVE_ERROR(xmax, v.back(),EPS);
        double delta = v[1]-v[0];
        for (size_t i = 2 ; i < n ; ++i)
        {
            ASSERT_LT(std::abs(delta), std::abs(v[i]-v[i-1]));
            delta = v[i]-v[i-1];
        }
    }
}

TEST_F(RadiationDampingBuilderTest, build_exponential_intervals_works_in_reverse)
{
    RadiationDampingBuilder builder(TypeOfInterpolation::SPLINES, TypeOfQuadrature::SIMPSON);
    for (size_t k = 0 ; k < 10 ; ++k)
    {
        const double xmin = a.random<double>();
        const double xmax = a.random<double>().no().greater_than(xmin);
        const size_t n = a.random<size_t>().between(3,200);
        const auto v = builder.build_exponential_intervals(xmin, xmax, n);
        ASSERT_EQ(n, v.size());
        ASSERT_SMALL_RELATIVE_ERROR(xmin, v.front(),EPS);
        ASSERT_SMALL_RELATIVE_ERROR(xmax, v.back(),EPS);
        double delta = v[1]-v[0];
        for (size_t i = 2 ; i < n ; ++i)
        {
            ASSERT_LT(std::abs(delta), std::abs(v[i]-v[i-1]));
            delta = v[i]-v[i-1];
        }
    }
}

TEST_F(RadiationDampingBuilderTest, build_exponential_intervals_works_with_opposite_signs)
{
    RadiationDampingBuilder builder(TypeOfInterpolation::SPLINES, TypeOfQuadrature::SIMPSON);
    for (size_t k = 0 ; k < 10 ; ++k)
    {
        const double xmin = a.random<double>().no().greater_than(0);
        const double xmax = a.random<double>().greater_than(0);
        const size_t n = a.random<size_t>().between(3,200);
        const auto v = builder.build_exponential_intervals(xmin, xmax, n);
        ASSERT_EQ(n, v.size());
        ASSERT_SMALL_RELATIVE_ERROR(xmin, v.front(),EPS);
        ASSERT_SMALL_RELATIVE_ERROR(xmax, v.back(),EPS);
        double delta = v[1]-v[0];
        for (size_t i = 2 ; i < n ; ++i)
        {
            ASSERT_LT(std::abs(delta), std::abs(v[i]-v[i-1]));
            delta = v[i]-v[i-1];
        }
    }
}

TEST_F(RadiationDampingBuilderTest, build_exponential_intervals_works_if_first_is_zero)
{
    RadiationDampingBuilder builder(TypeOfInterpolation::SPLINES, TypeOfQuadrature::SIMPSON);
    const auto v = builder.build_exponential_intervals(0, 10, 10);
    ASSERT_EQ(10, v.size());
    ASSERT_SMALL_RELATIVE_ERROR(0, v.front(),EPS);
    ASSERT_SMALL_RELATIVE_ERROR(10, v.back(),EPS);
    double delta = v[1]-v[0];
    for (size_t i = 2 ; i < 10 ; ++i)
    {
        ASSERT_LT(std::abs(delta), std::abs(v[i]-v[i-1]));
        delta = v[i]-v[i-1];
    }
}

TEST_F(RadiationDampingBuilderTest, build_exponential_intervals_works_if_last_is_zero)
{
    RadiationDampingBuilder builder(TypeOfInterpolation::SPLINES, TypeOfQuadrature::SIMPSON);
    const auto v = builder.build_exponential_intervals(-10, 0, 10);
    ASSERT_EQ(10, v.size());
    ASSERT_SMALL_RELATIVE_ERROR(-10, v.front(),EPS);
    ASSERT_SMALL_RELATIVE_ERROR(0, v.back(),EPS);
    double delta = v[1]-v[0];
    for (size_t i = 2 ; i < 10 ; ++i)
    {
        ASSERT_LT(std::abs(delta), std::abs(v[i]-v[i-1]));
        delta = v[i]-v[i-1];
    }
}

TEST_F(RadiationDampingBuilderTest, can_find_greatest_omega_for_which_integration_makes_sense)
{
    const double omega_min = 0;
    const double omega_max = 100;
    size_t N = 10;
    RadiationDampingBuilder builder(TypeOfInterpolation::SPLINES, TypeOfQuadrature::GAUSS_KRONROD);
    const auto omegas = builder.build_regular_intervals(omega_min, omega_max, N);
    double I0 = builder.integrate(test_data::analytical_Br, omega_min, omega_max);
    const double eps=1E-3;
    const double omega0 = builder.find_integration_bound(test_data::analytical_Br, omega_min, omega_max, eps);

    ASSERT_NEAR(eps,(builder.integrate(test_data::analytical_Br, omega_min, omega_max)-builder.integrate(test_data::analytical_Br, omega_min, omega0))/I0,EPS);
}

TEST_F(RadiationDampingBuilderTest, bug_detected_in_RadiationDampingForceModel)
{
    RadiationDampingBuilder builder(TypeOfInterpolation::SPLINES, TypeOfQuadrature::SIMPSON);
    const double tau_min = 0.1;
    const double tau_max = 40;
    const size_t n = 100;
    std::vector<double> taus = builder.build_regular_intervals(tau_min,tau_max,n);
    const auto K = builder.build_retardation_function(test_data::analytical_Br, taus, 1E-3);
    size_t N = 100;
    for (size_t i = 0 ; i < N ; ++i)
    {
        const double tau = tau_min + (tau_max-tau_min)*double(i)/double(N-1);
        const double K_analytical = exp(-0.1*tau)*cos(0.5*tau);
        ASSERT_SMALL_RELATIVE_ERROR(K_analytical, K(tau), 0.05) << "for tau = " << tau;
    }
}
