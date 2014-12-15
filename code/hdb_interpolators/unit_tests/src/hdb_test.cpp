/*
 * hdb_test.cpp
 *
 *  Created on: Dec 14, 2014
 *      Author: cady
 */

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include "hdb_data.hpp"
#include "hdb_test.hpp"
#include "RadiationDampingBuilder.hpp"

std::function<double(double)> get_interpolated_Br()
{
    std::vector<double> vBr;
    size_t N = 460;
    const double omega_min = 0.01;
    const double omega_max = 40;
    // LINEAR does not work. Need reverse + direct exponential intervals arround omega_med. Need 260 points.
    // Have to be careful not to miss the peak
    RadiationDampingBuilder builder(TypeOfQuadrature::SIMPSON, TypeOfQuadrature::SIMPSON);
    const double r = 0.5;
    const double omega_med = builder.find_r_bound(test_data::analytical_Br,omega_min,omega_max, r);
    auto omegas = builder.build_exponential_intervals_reversed(omega_min, omega_med, (size_t)std::floor((double)N*r+0.5));
    const auto omegas2 = builder.build_exponential_intervals(omega_med, omega_max, (size_t)std::floor((double)N*(1-r)+0.5));
    omegas.insert(omegas.end(), omegas2.begin()+1,omegas2.end());
    for (auto omega:omegas) vBr.push_back(test_data::analytical_Br(omega));
    return builder.build_interpolator(omegas,vBr);
}

std::function<double(double)> get_interpolated_K()
{
    size_t N = 50;
    const double omega_min = 0.;
    const double omega_max = 30;
    const double eps = 1E-8;
    RadiationDampingBuilder builder(TypeOfQuadrature::CLENSHAW_CURTIS, TypeOfQuadrature::CLENSHAW_CURTIS);
    const auto Br_ = get_interpolated_Br();


    auto taus = builder.build_regular_intervals(2*PI/omega_max,10,N);
    return builder.build_retardation_function(Br_,taus,eps,omega_min,omega_max);
}
