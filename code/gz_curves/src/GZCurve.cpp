/*
 * GZCurve.cpp
 *
 *  Created on: Feb 20, 2015
 *      Author: cady
 */

#include <cmath>
#include <sstream>

#include "GZCurve.hpp"
#include "GZException.hpp"

void check_input(const double dphi, const double phi_max);
void check_input(const double dphi, const double phi_max)
{
    std::stringstream error_msg;

    if (dphi<=0)      error_msg << "dphi cannot be negative: received dphi=" << dphi;
    if (phi_max<=0)   error_msg << "phi_max cannot be negative: received phi_max=" << phi_max;
    if (phi_max<dphi) error_msg << "phi_max cannot be less than dphi: received dphi=" << dphi << "and phi_max=" << phi_max;

    if (not(error_msg.str().empty()))
    {
        THROW(__PRETTY_FUNCTION__, GZException, error_msg.str());
    }
}

std::vector<double> compute_phi(const double dphi, const double phi_max);
std::vector<double> compute_phi(const double dphi, const double phi_max)
{
    std::vector<double> ret;
    const double N = std::round(phi_max/dphi);

    ret.push_back(-phi_max);
    for (int i = 1 ; i < 2*N ; ++i) ret.push_back((i-N)*dphi);
    ret.push_back(phi_max);

    return ret;
}

std::vector<double> GZ::Curve::get_phi(const double dphi, const double phi_max)
{
    check_input(dphi, phi_max);
    return compute_phi(dphi, phi_max);
}
