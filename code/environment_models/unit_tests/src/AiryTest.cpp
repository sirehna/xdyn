/*
 * AiryTest.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: cady
 */

#include "AiryTest.hpp"
#include "Airy.hpp"
#include "BretschneiderSpectrum.hpp"
#include "Cos2sDirectionalSpreading.hpp"
#include "DiracSpectralDensity.hpp"
#include "DiracDirectionalSpreading.hpp"
#include "SumOfWaveSpectralDensities.hpp"
#include "SumOfWaveDirectionalSpreadings.hpp"

#include "discretize.hpp"

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

AiryTest::AiryTest() : a(ssc::random_data_generator::DataGenerator(54200))
{
}

AiryTest::~AiryTest()
{
}

void AiryTest::SetUp()
{
}

void AiryTest::TearDown()
{
}

#define EPS 1E-10
#define BIG_EPS 1E-4


TEST_F(AiryTest, single_frequency_single_direction_at_one_point)
{
    //! [AiryTest example]
    const double psi0 = PI/4;
    const double Hs = 3;
    const double Tp = 10;
    const double omega0 = 2*PI/Tp;
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().greater_than(omega_min);
    const size_t nfreq = a.random<size_t>().between(2,100);
    const DiscreteDirectionalWaveSpectrum A = discretize(DiracSpectralDensity(omega0, Hs), DiracDirectionalSpreading(psi0), omega_min, omega_max, nfreq);
    int random_seed = 0;
    const Airy wave(A, random_seed);

    const double x = a.random<double>();
    const double y = a.random<double>();
    const double phi = 3.4482969340598712549;
    const double k = 4.*PI*PI/Tp/Tp/9.81;
    //! [AiryTest example]
    //! [AiryTest expected output]
    for (double t = 0 ; t < 3*Tp ; t+=0.1)
    {
        ASSERT_NEAR(Hs/2*cos(2*PI/Tp*t-k*(x*cos(psi0)+y*sin(psi0)) +phi), wave.elevation(x,y,t), 1E-6);
    }
    //! [AiryTest expected output]
}

TEST_F(AiryTest, two_frequencies_single_direction_at_one_point)
{
    const double psi0 = PI/4;
    const double Hs = 3;
    const double Tp = 10;
    const double omega0 = 2*PI/Tp;
    const double omega_min = 0.01;
    const double omega_max = 10;
    const size_t nfreq = 50;
    const DiscreteDirectionalWaveSpectrum A = discretize(DiracSpectralDensity(omega0, Hs)+DiracSpectralDensity(omega0, Hs), DiracDirectionalSpreading(psi0), omega_min, omega_max, nfreq);
    int random_seed = 0;
    const Airy wave(A, random_seed);

    const double x = a.random<double>();
    const double y = a.random<double>();
    const double phi = 3.4482969340598712549;
    const double k = 4.*PI*PI/Tp/Tp/9.81;
    for (double t = 0 ; t < 3*Tp ; t+=0.1)
    {
        ASSERT_NEAR(Hs/sqrt(2)*cos(2*PI/Tp*t - k*(x*cos(psi0)+y*sin(psi0)) +phi), wave.elevation(x,y,t), 1E-6);
    }
}

TEST_F(AiryTest, one_frequency_two_directions_at_one_point)
{
    const double psi0 = PI/4;
    const double Hs = 3;
    const double Tp = 10;
    const double omega0 = 2*PI/Tp;
    const double omega_min = 0.01;
    const double omega_max = 10;
    const size_t nfreq = 50;
    const DiscreteDirectionalWaveSpectrum A = discretize(DiracSpectralDensity(omega0, Hs), DiracDirectionalSpreading(psi0)+DiracDirectionalSpreading(psi0), omega_min, omega_max, nfreq);
    int random_seed = 0;
    const Airy wave(A, random_seed);

    const double x = a.random<double>();
    const double y = a.random<double>();
    const double phi = 3.4482969340598712549;
    const double k = 4.*PI*PI/Tp/Tp/9.81;
    for (double t = 0 ; t < 3*Tp ; t+=0.1)
    {
        ASSERT_NEAR(Hs/sqrt(2)*cos(2*PI/Tp*t - k*(x*cos(psi0)+y*sin(psi0)) +phi), wave.elevation(x,y,t), 1E-6);
    }
}

TEST_F(AiryTest, bug)
{
    const double psi0 = PI;
    const double Hs = 2;
    const double Tp = 7;
    const double s = 1;
    const double omega_min = 0.1;
    const double omega_max = 6;
    const size_t nfreq = 101;
    const DiscreteDirectionalWaveSpectrum A = discretize(BretschneiderSpectrum(Hs, Tp), Cos2sDirectionalSpreading(psi0, s), omega_min, omega_max, nfreq);
    int random_seed = 0;
    const Airy wave(A, random_seed);

    const double x = 0;
    const double y = 0;
    const double t = 0;

    ASSERT_LT(fabs(wave.elevation(x,y,t)), 5);

}

TEST_F(AiryTest, dynamic_pressure)
{
    const double psi0 = PI/4;
    const double Hs = 3;
    const double Tp = 10;
    const double omega0 = 2*PI/Tp;
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().greater_than(omega_min);
    const size_t nfreq = a.random<size_t>().between(2,100);
    const DiscreteDirectionalWaveSpectrum A = discretize(DiracSpectralDensity(omega0, Hs), DiracDirectionalSpreading(psi0), omega_min, omega_max, nfreq);
    int random_seed = 0;
    const Airy wave(A, random_seed);

    const double x = a.random<double>().between(-100,100);
    const double y = a.random<double>().between(-100,100);
    const double z = a.random<double>().between(0,100);
    const double phi = 3.4482969340598712549;
    const double rho = 1024;
    const double g = 9.81;
    const double k = 4.*PI*PI/Tp/Tp/g;

    for (double t = 0 ; t < 3*Tp ; t+=0.1)
    {
        const double eta = 0;
        ASSERT_NEAR(Hs/2*rho*g*exp(-k*z)*cos(2*PI/Tp*t - k*(x*cos(psi0)+y*sin(psi0)) +phi), wave.dynamic_pressure(rho,g,x,y,z,eta,t), 1E-6);
    }
}

TEST_F(AiryTest, validate_elevation_formula_against_results_from_sos)
{
    const double Hs = 0.1;
    const double Tp = 5;
    const double omega0 = 2*PI/Tp;
    double psi = 0;
    double phi = 5.8268;
    double t = 0;
    const double g = 9.81;
    const double k = omega0*omega0/g;
    double x=-0.1; double y=0;
    ASSERT_NEAR(0.045232, Hs/2*cos(omega0*t-k*(x*cos(psi)+y*sin(psi))+phi), BIG_EPS);
    x=0.1;y=0;
    ASSERT_NEAR(0.044522, Hs/2*cos(omega0*t-k*(x*cos(psi)+y*sin(psi))+phi), BIG_EPS);
    x=0;y=-0.1;
    ASSERT_NEAR(0.044883, Hs/2*cos(omega0*t-k*(x*cos(psi)+y*sin(psi))+phi), BIG_EPS);
    x=0;y=0.1;
    ASSERT_NEAR(0.044883, Hs/2*cos(omega0*t-k*(x*cos(psi)+y*sin(psi))+phi), BIG_EPS);
    x=0;y=0;
    ASSERT_NEAR(0.044883, Hs/2*cos(omega0*t-k*(x*cos(psi)+y*sin(psi))+phi), BIG_EPS);
}

TEST_F(AiryTest, should_be_able_to_reproduce_results_from_sos_stab)
{
    const double Hs = 0.1;
    const double Tp = 5;
    const double omega0 = 2*PI/Tp;
    double psi = 0;
    double phi = 5.8268;
    double t = 0;

    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().greater_than(omega_min);
    const size_t nfreq = a.random<size_t>().between(2,100);
    const DiscreteDirectionalWaveSpectrum A = discretize(DiracSpectralDensity(omega0, Hs), DiracDirectionalSpreading(psi), omega_min, omega_max, nfreq);
    const Airy wave(A, phi);

    double x=-0.1; double y=0;
    ASSERT_NEAR(0.045232, wave.elevation(x,y,t), BIG_EPS);
    x=0.1;y=0;
    ASSERT_NEAR(0.044522, wave.elevation(x,y,t), BIG_EPS);
    x=0;y=-0.1;
    ASSERT_NEAR(0.044883, wave.elevation(x,y,t), BIG_EPS);
    x=0;y=0.1;
    ASSERT_NEAR(0.044883, wave.elevation(x,y,t), BIG_EPS);
    x=0;y=0;
    ASSERT_NEAR(0.044883, wave.elevation(x,y,t), BIG_EPS);
}

TEST_F(AiryTest, validate_dynamic_pressure_formula_against_sos_stab)
{
    const double Hs = 0.1;
    const double Tp = 5;
    const double omega0 = 2*PI/Tp;
    double psi = 0;
    double phi = 5.8268;
    double t = 0;
    const double g = 9.81;
    const double k = omega0*omega0/g;

    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().greater_than(omega_min);
    const size_t nfreq = a.random<size_t>().between(2,100);
    const DiscreteDirectionalWaveSpectrum A = discretize(DiracSpectralDensity(omega0, Hs), DiracDirectionalSpreading(psi), omega_min, omega_max, nfreq);
    const Airy wave(A, phi);

    double x=-0.1; double y=0; double z=0.2;
    ASSERT_NEAR(0.044118, Hs/2*exp(-k*(z-wave.elevation(x,y,t)))*cos(omega0*t-k*(x*cos(psi)+y*sin(psi))+phi), BIG_EPS);
    x=0.1;y=0;z=0.2;
    ASSERT_NEAR(0.04342, Hs/2*exp(-k*(z-wave.elevation(x,y,t)))*cos(omega0*t-k*(x*cos(psi)+y*sin(psi))+phi), BIG_EPS);
    x=0;y=-0.1;z=0.2;
    ASSERT_NEAR(0.043775, Hs/2*exp(-k*(z-wave.elevation(x,y,t)))*cos(omega0*t-k*(x*cos(psi)+y*sin(psi))+phi), BIG_EPS);
    x=0;y=0.1;z=0.2;
    ASSERT_NEAR(0.043775, Hs/2*exp(-k*(z-wave.elevation(x,y,t)))*cos(omega0*t-k*(x*cos(psi)+y*sin(psi))+phi), BIG_EPS);
    x=0;y=0;z=0.1;
    ASSERT_NEAR(0.044486, Hs/2*exp(-k*(z-wave.elevation(x,y,t)))*cos(omega0*t-k*(x*cos(psi)+y*sin(psi))+phi), BIG_EPS);
    x=0;y=0;z=0.3;
    ASSERT_NEAR(0.043075, Hs/2*exp(-k*(z-wave.elevation(x,y,t)))*cos(omega0*t-k*(x*cos(psi)+y*sin(psi))+phi), BIG_EPS);
}

TEST_F(AiryTest, dynamic_pressure_compare_with_sos_stab)
{
    const double Hs = 0.1;
    const double Tp = 5;
    const double omega0 = 2*PI/Tp;
    double psi = 0;
    double phi = 5.8268;
    double t = 0;
    const double rho = 1025;
    const double g = 9.81;

    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().greater_than(omega_min);
    const size_t nfreq = a.random<size_t>().between(2,100);
    const DiscreteDirectionalWaveSpectrum A = discretize(DiracSpectralDensity(omega0, Hs), DiracDirectionalSpreading(psi), omega_min, omega_max, nfreq);
    const Airy wave(A, phi);

    double x=-0.1; double y=0; double z=0.2; double eta = wave.elevation(x,y,t);
    ASSERT_NEAR(0.044118, wave.dynamic_pressure(rho,g,x,y,z,eta,t)/rho/g, BIG_EPS);
    x=0.1;y=0;z=0.2; eta = wave.elevation(x,y,t);
    ASSERT_NEAR(0.04342, wave.dynamic_pressure(rho,g,x,y,z,eta,t)/rho/g, BIG_EPS);
    x=0;y=-0.1;z=0.2; eta = wave.elevation(x,y,t);
    ASSERT_NEAR(0.043775, wave.dynamic_pressure(rho,g,x,y,z,eta,t)/rho/g, BIG_EPS);
    x=0;y=0.1;z=0.2;eta = wave.elevation(x,y,t);
    ASSERT_NEAR(0.043775, wave.dynamic_pressure(rho,g,x,y,z,eta,t)/rho/g, BIG_EPS);
    x=0;y=0;z=0.1;eta = wave.elevation(x,y,t);
    ASSERT_NEAR(0.044486, wave.dynamic_pressure(rho,g,x,y,z,eta,t)/rho/g, BIG_EPS);
    x=0;y=0;z=0.3;eta = wave.elevation(x,y,t);
    ASSERT_NEAR(0.043075, wave.dynamic_pressure(rho,g,x,y,z,eta,t)/rho/g, BIG_EPS);
}

TEST_F(AiryTest, norm_of_orbital_velocity_should_only_depend_on_z)
{
    const double Hs = 0.1;
    const double Tp = 5;
    const double omega0 = 2*PI/Tp;
    double psi = 0;
    double phi = 5.8268;
    double t = 0;
    const double g = 9.81;

    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().greater_than(omega_min);
    const size_t nfreq = a.random<size_t>().between(2,100);
    const DiscreteDirectionalWaveSpectrum A = discretize(DiracSpectralDensity(omega0, Hs), DiracDirectionalSpreading(psi), omega_min, omega_max, nfreq);
    const Airy wave(A, phi);

    ssc::kinematics::Point V;
    for (double z = 100 ; z < 200 ; z+=100)
    {
        double x=0; double y=0;
        ssc::kinematics::Point V = wave.orbital_velocity(g,x,y,z,t);
        const double ref = std::sqrt(V.x()*V.x() + V.y()*V.y() + V.z()*V.z());
        for (size_t i = 0 ; i < 100 ; ++i)
        {
            x = a.random<double>().between(-100,100);
            y = a.random<double>().between(-100,100);
            V = wave.orbital_velocity(g,x,y,z,t);
            const double norm = std::sqrt(V.x()*V.x() + V.y()*V.y() + V.z()*V.z());
            ASSERT_NEAR(ref, norm, EPS);
        }
    }
}

TEST_F(AiryTest, orbital_velocity_non_regression_test)
{
    const double Hs = 0.1;
    const double Tp = 5;
    const double omega0 = 2*PI/Tp;
    double psi = PI/3;
    double phi = 5.8268;
    double t = 0;
    const double g = 9.81;

    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().greater_than(omega_min);
    const size_t nfreq = a.random<size_t>().between(2,100);
    const DiscreteDirectionalWaveSpectrum A = discretize(DiracSpectralDensity(omega0, Hs), DiracDirectionalSpreading(psi), omega_min, omega_max, nfreq);
    const Airy wave(A, phi);

    ssc::kinematics::Point V;
    double x=-0.1; double y=0; double z=0.2;
    V = wave.orbital_velocity(g,x,y,z,t);
    ASSERT_EQ("NED", V.get_frame());
    EXPECT_NEAR(0.027414126666006417, V.x(), EPS);
    EXPECT_NEAR(0.047482660230651896, V.y(), EPS);
    EXPECT_NEAR(-0.026372680043643015, V.z(), EPS);
    x=0.1;y=0;z=0.2;
    V = wave.orbital_velocity(g,x,y,z,t);
    EXPECT_EQ("NED", V.get_frame());
    EXPECT_NEAR(0.02719832079249054, V.x(), EPS);
    EXPECT_NEAR(0.047108873493150613, V.y(), EPS);
    EXPECT_NEAR(-0.02725180728683603, V.z(), EPS);
    x=0;y=-0.1;z=0.2;
    V = wave.orbital_velocity(g,x,y,z,t);
    EXPECT_EQ("NED", V.get_frame());
    EXPECT_NEAR(0.027491344144018031, V.x(), EPS);
    EXPECT_NEAR(0.047616404825800346, V.y(), EPS);
    EXPECT_NEAR(-0.026049176661545468, V.z(), EPS);
    x=0;y=0.1;z=0.2;
    V = wave.orbital_velocity(g,x,y,z,t);
    EXPECT_EQ("NED", V.get_frame());
    EXPECT_NEAR(0.02711756547781036, V.x(), EPS);
    EXPECT_NEAR(0.046969001185143334, V.y(), EPS);
    EXPECT_NEAR(-0.027571836833087733, V.z(), EPS);
    x=0;y=0;z=0.1;
    V = wave.orbital_velocity(g,x,y,z,t);
    EXPECT_EQ("NED", V.get_frame());
    EXPECT_NEAR(0.027750233543419611, V.x(), EPS);
    EXPECT_NEAR(0.048064814419104873, V.y(), EPS);
    EXPECT_NEAR(-0.027248221171516328, V.z(), EPS);
    x=0;y=0;z=0.3;
    V = wave.orbital_velocity(g,x,y,z,t);
    EXPECT_EQ("NED", V.get_frame());
    EXPECT_NEAR(0.026871058840978748, V.x(), EPS);
    EXPECT_NEAR(0.046542039165748048, V.y(), EPS);
    EXPECT_NEAR(-0.026384951076760983, V.z(), EPS);
}

TEST_F(AiryTest, orbital_velocity_sanity_check)
{
    const double Hs = 0.1;
    const double Tp = 5;
    const double omega0 = 2*PI/Tp;

    double t = 0;
    const double g = 9.81;

    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().greater_than(omega_min);
    const size_t nfreq = a.random<size_t>().between(2,100);


    for (size_t i = 0 ; i < 100 ; ++i)
    {
        const double psi = a.random<double>().between(-PI, PI);
        const DiscreteDirectionalWaveSpectrum A = discretize(DiracSpectralDensity(omega0, Hs), DiracDirectionalSpreading(psi), omega_min, omega_max, nfreq);
        const Airy wave(A, a.random<double>().between(-PI,PI));

        double x=a.random<double>().between(-100,100); double y=a.random<double>().between(-100,100); double z=a.random<double>().between(-100,100);
        const ssc::kinematics::Point V = wave.orbital_velocity(g,x,y,z,t);
        ASSERT_NEAR(std::abs(cos(psi)),std::abs(V.x()/hypot(V.x(),V.y())), EPS);
        ASSERT_NEAR(std::abs(sin(psi)),std::abs(V.y()/hypot(V.x(),V.y())), EPS);
    }
}

TEST_F(AiryTest, should_get_different_results_when_using_two_different_spectra)
{
    const double Hs = 0.1;
    const double Tp = 5;
    const double omega0 = 2*PI/Tp;
    const double psi = 2*PI/3;
    const double omega_min = 0;
    const double omega_max = 10;
    const size_t nfreq = 11;
    const DiscreteDirectionalWaveSpectrum A1 = discretize(DiracSpectralDensity(omega0, Hs), DiracDirectionalSpreading(psi), omega_min, omega_max, nfreq);
    const DiscreteDirectionalWaveSpectrum A2 = discretize(DiracSpectralDensity(omega0, 10*Hs), DiracDirectionalSpreading(psi), omega_min, omega_max, nfreq);
    const Airy wave1(A1, 0);
    const Airy wave2(A2, 0);
    const std::vector<std::vector<double> > rao_module = {{1,2,3},{4,5,6},{7,8,9},{0,1,2},{3,4,5},{6,7,8},{9,0,1},{2,3,4},{5,6,7},{8,9,0}};
    const std::vector<std::vector<double> > rao_phase = {{9,8,7},{6,5,4},{1,4,7},{8,5,2},{7,5,3},{1,5,9},{4,5,6},{7,8,9},{6,5,4},{4,8,6}};

    ASSERT_GT(std::abs(wave1.evaluate_rao(4,5,6,rao_module,rao_phase)-wave2.evaluate_rao(4,5,6,rao_module,rao_phase)), 1E-6);
}

TEST_F(AiryTest, RAO_non_regression_test)
{
    const double Hs = 0.1;
    const double Tp = 5;
    const double omega0 = 2*PI/Tp;
    const double psi = 2*PI/3;
    const double omega_min = 0;
    const double omega_max = 10;
    const size_t nfreq = 11;
    const DiscreteDirectionalWaveSpectrum A = discretize(DiracSpectralDensity(omega0, Hs), DiracDirectionalSpreading(psi), omega_min, omega_max, nfreq);
    const Airy wave(A, 0);
    const std::vector<std::vector<double> > rao_module = {{1,2,3},{4,5,6},{7,8,9},{0,1,2},{3,4,5},{6,7,8},{9,0,1},{2,3,4},{5,6,7},{8,9,0}};
    const std::vector<std::vector<double> > rao_phase = {{9,8,7},{6,5,4},{1,4,7},{8,5,2},{7,5,3},{1,5,9},{4,5,6},{7,8,9},{6,5,4},{4,8,6}};
    ASSERT_NEAR(0.036121783468892797,wave.evaluate_rao(4,5,6,rao_module,rao_phase), 1E-6);
}
