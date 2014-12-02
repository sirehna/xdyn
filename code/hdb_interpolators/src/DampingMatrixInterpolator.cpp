/*
 * DampingMatrixInterpolator.cpp
 *
 *  Created on: Nov 27, 2014
 *      Author: cady
 */

#include <ssc/macros/tr1_macros.hpp>
#include <ssc/integrate.hpp>
#include TR1INC(memory)

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include <ssc/interpolation.hpp>
typedef TR1(shared_ptr)<ssc::interpolation::Interpolator> InterpolatorPtr;
typedef TR1(shared_ptr)<ssc::integrate::Integrator> IntegratorPtr;

#include "DampingMatrixInterpolator.hpp"
#include "DampingMatrixInterpolatorException.hpp"

RadiationDampingBuilder::RadiationDampingBuilder(const TypeOfInterpolation& type_of_interpolation_, const TypeOfQuadrature& type_of_quadrature_) : type_of_interpolation(type_of_interpolation_), type_of_quadrature(type_of_quadrature_)
{
}

std::function<double(double)> RadiationDampingBuilder::build_interpolator(const std::vector<double>& x, const std::vector<double>& y) const
{
    InterpolatorPtr i;
    switch(type_of_interpolation)
    {
        case TypeOfInterpolation::LINEAR:
            i.reset(new ssc::interpolation::LinearInterpolationVariableStep(x, y));
            break;
        case TypeOfInterpolation::PIECEWISE_CONSTANT:
            i.reset(new ssc::interpolation::PiecewiseConstantVariableStep<double>(x, y));
            break;
        case TypeOfInterpolation::SPLINES:
            i.reset(new ssc::interpolation::SplineVariableStep(x, y));
            break;
        default:
            THROW(__PRETTY_FUNCTION__, DampingMatrixInterpolatorException, "Unknown type of interpolation.");
            break;
    }
    std::function<double(double)> ret = [i](const double x){return i->f(x);};
    return ret;
}

double RadiationDampingBuilder::integrate(const std::function<double(double)>& Br, const double tau, const double omega_min, const double omega_max) const
{
    IntegratorPtr i;
    const auto f = [&Br,tau](const double omega){return Br(omega)*cos(omega*tau);};
    switch (type_of_quadrature)
    {
        case TypeOfQuadrature::GAUSS_KRONROD:
            i.reset(new ssc::integrate::GaussKronrod(f));
            break;
        case TypeOfQuadrature::RECTANGLE:
            i.reset(new ssc::integrate::Rectangle(f));
            break;
        case TypeOfQuadrature::SIMPSON:
            i.reset(new ssc::integrate::Simpson(f));
            break;
        case TypeOfQuadrature::TRAPEZOIDAL:
            i.reset(new ssc::integrate::TrapezoidalIntegration(f));
            break;
        default:
            THROW(__PRETTY_FUNCTION__, DampingMatrixInterpolatorException, "Unknown type of quadrature.");
            break;
    }
    return 2./PI*i->integrate_f(omega_min, omega_max);
}

std::function<double(double)> RadiationDampingBuilder::make_retardation_function(const std::function<double(double)>& Br, const double omega_min, const double omega_max, const size_t n) const
{
    std::vector<double> x(n,0), y(n, 0);
    const double tau_min = 2*PI/omega_max;
    const double tau_max = 2*PI/omega_min;
    for (size_t i = 0 ; i < n ; ++i)
    {
        const double tau = tau_min + (tau_max - tau_min)*(double)i/((double)n-1.);
        x[i] = tau;
        y[i] = integrate(Br, tau, omega_min, omega_max);
    }
    return build_interpolator(x, y);
}
