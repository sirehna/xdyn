/*
 * RadiationDampingBuilder.cpp
 *
 *  Created on: Nov 27, 2014
 *      Author: cady
 */

#include <boost/math/tools/roots.hpp>

#include <ssc/macros/tr1_macros.hpp>
#include <ssc/integrate.hpp>
#include TR1INC(memory)

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include <ssc/interpolation.hpp>
typedef TR1(shared_ptr)<ssc::interpolation::Interpolator> InterpolatorPtr;
typedef TR1(shared_ptr)<ssc::integrate::Integrator> IntegratorPtr;

#include "DampingMatrixInterpolatorException.hpp"
#include "History.hpp"
#include "RadiationDampingBuilder.hpp"

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
    const auto f = [&Br,tau](const double omega){return Br(omega)*cos(omega*tau);};
    return 2./PI*integrate(f, omega_min, omega_max);
}

double RadiationDampingBuilder::integrate(const std::function<double(double)>& f, const double a, const double b) const
{
    switch (type_of_quadrature)
    {
        case TypeOfQuadrature::GAUSS_KRONROD:
            return ssc::integrate::GaussKronrod(f).integrate_f(a,b);
            break;
        case TypeOfQuadrature::RECTANGLE:
            return ssc::integrate::Rectangle(f).integrate_f(a, b);
            break;
        case TypeOfQuadrature::SIMPSON:
            return ssc::integrate::Simpson(f).integrate_f(a, b);
            break;
        case TypeOfQuadrature::TRAPEZOIDAL:
            return ssc::integrate::TrapezoidalIntegration(f).integrate_f(a, b);
            break;
        default:
            THROW(__PRETTY_FUNCTION__, DampingMatrixInterpolatorException, "Unknown type of quadrature.");
            break;
    }
    return 0;
}

std::function<double(double)> RadiationDampingBuilder::build_retardation_function(const std::function<double(double)>& Br, const std::vector<double>& taus) const
{
    const double omega_min = 2*PI/taus.back();
    const double omega_max = 2*PI/taus.front();
    std::vector<double> y;
    for (auto tau:taus) y.push_back(integrate(Br, tau, omega_min, omega_max));
    return build_interpolator(taus, y);;
}

double RadiationDampingBuilder::convolution(const History& h, //!< State history
                           const std::function<double(double)>& f, //!< Function to convolute with
                           const double Tmin, //!< Beginning of the convolution (because retardation function may not be defined for T=0)
                           const double Tmax  //!< End of the convolution
                           ) const
{
    const auto g = [&h, &f](const double tau){return h.get(tau)*f(tau);};
    return integrate(g, Tmin, Tmax);
}

std::vector<double> RadiationDampingBuilder::build_regular_intervals(const double first, //!< First value in vector
                                                    const double last, //!< Last value in vector
                                                    const size_t n //!< Number of values to return
                                                    ) const
{
    std::vector<double> ret(n);
    for (size_t i = 0 ; i < n ; ++i)
    {
        ret[i] = first + (last-first)*(double)(i)/((double)n-1);
    }
    return ret;
}

double RadiationDampingBuilder::find_integration_bound(const std::function<double(double)>& f, //!< Function to integrate
                                                       const double omega_min,                 //!< Lower bound of the integration (returned omega is necessarily greater than omega_min)
                                                       const double omega_max,                 //!< Upper bound of the integration (returned omega is necessarily lower than omega_min)
                                                       const double eps                        //!< Integration error (compared to full integration from omega_min up to omega_max)
                                                       ) const
{
    boost::math::tools::eps_tolerance<double> tol(30);
    const double I0 = integrate(f, omega_min, omega_max);
    const auto g = [&f,I0,this,eps,omega_min,omega_max](const double omega){return integrate(f, omega_min, omega)-(1-eps)*I0;};
    boost::uintmax_t max_iter=20;
    return boost::math::tools::toms748_solve(g, omega_min, omega_max, tol, max_iter).first;
}
