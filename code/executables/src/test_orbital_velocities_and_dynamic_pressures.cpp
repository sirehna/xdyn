/*
 *  test_orbital_velocities_and_dynamic_pressures.cpp
 *
 *  Created on: Jan 3, 2017
 *      Author: cady
 */
#include <vector> // Needs to be declared before ssc/macros.hpp to overload <<
#include "Airy.hpp"
#include "DiracSpectralDensity.hpp"
#include "DiracDirectionalSpreading.hpp"
#include "Stretching.hpp"
#include "discretize.hpp"
#include "YamlWaveModelInput.hpp"
#include <ssc/random_data_generator.hpp>
#include <ssc/macros.hpp>

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

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


    const double xmin = -20;
    const double xmax = 20;
    const double zmin = -2;
    const double zmax = 30;

    const size_t nx = 80;
    const size_t nz = 640;

    double t = 0;
    std::vector<double> pdyn; pdyn.reserve(nx*nz);
    std::vector<double> uorb(nx*nz);
    std::vector<double> vorb(nx*nz);
    std::vector<double> worb(nx*nz);
    std::vector<double> x(nx);
    std::vector<double> y(nx, 0);
    std::vector<double> z(nz);
    std::vector<double> usurf(nx); //!< u-component of the orbital velocity, taken on the air-water interface (free surface)
    std::vector<double> wsurf(nx); //!< w-component of the orbital velocity, taken on the air-water interface (free surface)

    for (size_t i = 0; i < nx; ++i)
    {
        x.at(i) = xmin + (xmax - xmin) * ((double)i) / ((double)nx - 1.);
    }

    std::vector<double> eta = wave.get_elevation(x, y, t);

    for (size_t j = 0; j < nz; ++j)
    {
        z.at(j) = zmin + (zmax - zmin) * ((double)j) / ((double)nz - 1.);
    }

    const ssc::kinematics::PointMatrix Vsurf = wave.orbital_velocity(g, x, y, eta, t, eta);
    for (size_t i = 0; i < nx; ++i)
    {
        usurf.at(i) = Vsurf.m(0, i);
        wsurf.at(i) = Vsurf.m(2, i);

        const std::vector<double> pdyn_i = wave.get_dynamic_pressure(rho, g, std::vector<double>(nz, x.at(i)), std::vector<double>(nz, y.at(i)), z, std::vector<double>(nz, eta.at(i)), t);
        pdyn.insert(pdyn.begin() + nz * i, pdyn_i.begin(), pdyn_i.end());
        
        const ssc::kinematics::PointMatrix V = wave.orbital_velocity(g, std::vector<double>(nz, x.at(i)), std::vector<double>(nz, y.at(i)), z, t, std::vector<double>(nz, eta.at(i)));
        for (size_t j = 0 ; j < nz ; ++j)
        {
            uorb.at(nz*i+j) = V.m(0, j);
            vorb.at(nz*i+j) = V.m(1, j);
            worb.at(nz*i+j) = V.m(2, j);
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
              << ", \"usurf\": " << usurf
              << ", \"wsurf\": " << wsurf
              << ", \"depth\": " << h
              << ", \"Hs\": " << Hs
              << ", \"psi\": " << psi*180/PI
              << ", \"Tp\": " << Tp
              << ", \"h\": " << ys.h
              << ", \"delta\": " << ys.delta
              << "}\n";

    return 0;
}
