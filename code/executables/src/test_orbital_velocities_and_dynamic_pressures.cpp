/*
 * test_orbital_velocities_and_dynamic_pressures.cpp
 *
 *  Created on: Jan 3, 2017
 *      Author: cady
 */
#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include <ssc/random_data_generator.hpp>

#include "Airy.hpp"
//#include "BretschneiderSpectrum.hpp"
//#include "Cos2sDirectionalSpreading.hpp"
#include "DiracSpectralDensity.hpp"
#include "DiracDirectionalSpreading.hpp"
//#include "SumOfWaveSpectralDensities.hpp"
//#include "SumOfWaveDirectionalSpreadings.hpp"
#include "discretize.hpp"
#include "YamlWaveModelInput.hpp"

int main(int , char** )
{
    const double Hs = 3;
    const double Tp = 5;
    const double psi = 0;
    const double omega0 = 2*PI/Tp;
    const double g = 9.81;
    const double rho = 1000;

    ssc::random_data_generator::DataGenerator a(123);
    const double omega_min = 0.1;
    const double omega_max = 10;
    const size_t nfreq = 100;

    YamlStretching ys;
    ys.h = 20;//10;
    ys.delta = 1;//0;
    const double h = 20;
    const Stretching stretching(ys);
    const DiscreteDirectionalWaveSpectrum A = discretize(DiracSpectralDensity(omega0, Hs), DiracDirectionalSpreading(psi), omega_min, omega_max, nfreq, h, stretching);
    const double random_phase = a.random<double>().between(-PI,PI);
    const Airy wave(A, random_phase);


    const double xmin = -25;
    const double xmax = 25;
    const double zmin = -5;
    const double zmax = 40;
    const double y = 0;

    const size_t nx = 400;
    const size_t nz = 80;

    double t = 0;
    std::vector<double> pdyn(nx*nz);
    std::vector<double> uorb(nx*nz);
    std::vector<double> vorb(nx*nz);
    std::vector<double> worb(nx*nz);
    std::vector<double> x(nx);
    std::vector<double> z(nz);
    std::vector<double> eta(nx);
    for (size_t i = 0 ; i < nx ; ++i)
    {
        x.at(i) = xmin + (xmax - xmin)* ((double)i)/((double)nx - 1.);
        eta.at(i) = wave.elevation(x.at(i), y, t);
        for (size_t j = 0 ; j < nz ; ++j)
        {
            z.at(j) = zmin + (zmax - zmin)* ((double)j)/((double)nz - 1.);
            pdyn.at(nz*i+j) = wave.dynamic_pressure(rho, g, x.at(i), y, z.at(j), eta.at(i), t);
            const ssc::kinematics::Point V = wave.orbital_velocity(g,x.at(i),y,z.at(j),t,eta.at(i));
            uorb.at(nz*i+j) = V.v(0);
            vorb.at(nz*i+j) = V.v(1);
            worb.at(nz*i+j) = V.v(2);
        }
    }
    std::cout << "{\"nx\": " << nx
              << ", \"nz\": " << nz
              << ", \"x\": " << x
              << ", \"z\": " << z
              << ", \"eta\": " << eta
              << ", \"pdyn\": " << pdyn
              << ", \"uorb\": " << uorb
              << ", \"vorb\": " << vorb
              << ", \"worb\": " << worb
              << ", \"depth\": " << h
              << ", \"Hs\": " << Hs
              << ", \"psi\": " << psi*180/PI
              << ", \"Tp\": " << Tp
              << ", \"h\": " << ys.h
              << ", \"delta\": " << ys.delta
              << "}\n";

    return 0;
}
