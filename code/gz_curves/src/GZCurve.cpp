/*
 * GZCurve.cpp
 *
 *  Created on: Feb 20, 2015
 *      Author: cady
 */

#include <cmath>
#include <sstream>

#include "GZCurve.hpp"
#include "InvalidInputException.hpp"
#include "InternalErrorException.hpp"
#include "gz_newton_raphson.hpp"
#include "ResultantForceComputer.hpp"
#include "Sim.hpp"

struct GZ::Curve::Impl
{
    Impl(const Sim& sim) :
        res(sim),
        f(std::bind(&ResultantForceComputer::resultant, &res, std::placeholders::_1)),
        k(std::bind(&ResultantForceComputer::K, &res, std::placeholders::_1))
    {
        std::vector<double> x(13, 0);
        sim.get_bodies().front()->update(sim.get_env(),x,0);
        const double total_volume = sim.get_bodies().front()->get_states().intersector->emerged_volume() +  sim.get_bodies().front()->get_states().intersector->immersed_volume();
        const double weight_of_water_displaced_by_fully_immersed_body = sim.get_env().rho*total_volume;
        const double body_mass = sim.get_bodies().front()->get_states().solid_body_inertia->operator()(2,2);
        if (body_mass > weight_of_water_displaced_by_fully_immersed_body)
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "The body is denser than water & will sink: weight of water displaced by fully immersed_body is " << weight_of_water_displaced_by_fully_immersed_body << " kg, but body mass is " << body_mass << " kg.");
        }
    }

    double FZ(const double z, const double phi, const double theta)
    {
        return res.resultant(State(z, phi, theta)).state(0);
    }

    ResultantForceComputer res;
    FType f;
    KComputer k;
};

void check_input(const double dphi, const double phi_max);
void check_input(const double dphi, const double phi_max)
{
    std::stringstream error_msg;

    if (dphi<=0)      error_msg << "dphi cannot be negative: received dphi=" << dphi;
    if (phi_max<=0)   error_msg << "phi_max cannot be negative: received phi_max=" << phi_max;
    if (phi_max<dphi) error_msg << "phi_max cannot be less than dphi: received dphi=" << dphi << "and phi_max=" << phi_max;

    if (not(error_msg.str().empty()))
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, error_msg.str());
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

GZ::State GZ::Curve::get_Xeq() const
{
    const double z0 = zeq(0,0);
    const GZ::State x0(z0, 0, 0);
    const auto g = GZ::newton_raphson(x0, pimpl->f, pimpl->k, 100, 1E-6);
    return x0;
}

GZ::Curve::Curve(const Sim& sim) : pimpl(new Impl(sim)), theta_eq(get_Xeq()(2))
{
}

std::vector<double> GZ::Curve::get_phi(const double dphi, const double phi_max)
{
    check_input(dphi, phi_max);
    return compute_phi(dphi, phi_max);
}

double delta(const GZ::MinMax& z);
double delta(const GZ::MinMax& z)
{
    return z.max-z.min;
}

double average(const GZ::MinMax& z);
double average(const GZ::MinMax& z)
{
    return 0.5*(z.min+z.max);
}

double GZ::Curve::zeq(const double phi, const double theta) const
{
    auto z = pimpl->res.get_zmin_zmax(phi);
    MinMax FZ(pimpl->FZ(z.max,phi,theta), pimpl->FZ(z.min,phi,theta));
    if (FZ.min>0)
    {
        return z.min;
    }
    if (FZ.max<0)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Resultant should be oriented downwards when body is fully emerged");
    }
    while (delta(z)>1E-10)
    {
        const double z0 = average(z);
        double FZ0 = pimpl->FZ(average(z),phi,theta);
        z = (FZ0 < 0) ? MinMax(z.min,z0) : MinMax(z0,z.max);
    }
    return average(z);
}

double GZ::Curve::gz(const double phi) const
{
    const double z_eq = zeq(phi, theta_eq);
    const GZ::State Xeq(z_eq, phi, theta_eq);
    return pimpl->res.resultant(Xeq).gz;
}

double GZ::Curve::get_theta_eq() const
{
    return theta_eq;
}
