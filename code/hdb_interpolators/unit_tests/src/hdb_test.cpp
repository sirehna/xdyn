/*
 * hdb_test.cpp
 *
 *  Created on: Dec 14, 2014
 *      Author: cady
 */

#include <cmath>

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
    RadiationDampingBuilder builder(TypeOfInterpolation::SPLINES, TypeOfQuadrature::SIMPSON);
    const double r = 0.5;
    const double omega_med = builder.find_r_bound(test_data::analytical_Br,omega_min,omega_max, r);
    auto omegas = builder.build_exponential_intervals_reversed(omega_min, omega_med, (size_t)std::floor(N*r+0.5));
    const auto omegas2 = builder.build_exponential_intervals(omega_med, omega_max, (size_t)std::floor(N*(1-r)+0.5));
    omegas.insert(omegas.end(), omegas2.begin()+1,omegas2.end());
    for (auto omega:omegas) vBr.push_back(test_data::analytical_Br(omega));
    return builder.build_interpolator(omegas,vBr);
}


