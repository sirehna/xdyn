/*
 * RadiationDampingBuilder.cpp
 *
 *  Created on: Nov 27, 2014
 *      Author: cady
 */

#include <algorithm>    // std::reverse

#include <boost/math/tools/roots.hpp>

#include <ssc/macros/tr1_macros.hpp>
#include <ssc/interpolation.hpp>
#include <ssc/integrate.hpp>
typedef TR1(shared_ptr)<ssc::interpolation::Interpolator> InterpolatorPtr;

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include "History.hpp"
#include "DampingMatrixInterpolatorException.hpp"
#include "RadiationDampingBuilder.hpp"

RadiationDampingBuilder::RadiationDampingBuilder(const TypeOfQuadrature& type_of_quadrature_for_convolution_, //!< Gauss-Kronrod, rectangle, Simpson, trapezoidal, Burcher, Clenshaw-Curtis or Filon
                                                 const TypeOfQuadrature& type_of_quadrature_for_cos_transform_ //!< Gauss-Kronrod, rectangle, Simpson, trapezoidal, Burcher, Clenshaw-Curtis or Filon
        ): type_of_quadrature_for_convolution(type_of_quadrature_for_convolution_),
           type_of_quadrature_for_cos_transform(type_of_quadrature_for_cos_transform_)
{
}

std::function<double(double)> RadiationDampingBuilder::build_interpolator(const std::vector<double>& x, const std::vector<double>& y) const
{
    InterpolatorPtr i;
    const bool allow_queries_outside_bounds = true;
    i.reset(new ssc::interpolation::SplineVariableStep(x, y, allow_queries_outside_bounds));
    std::function<double(double)> ret = [i](const double x){return i->f(x);};
    return ret;
}

double RadiationDampingBuilder::cos_transform(const std::function<double(double)>& Br, const double a, const double b, const double tau) const
{
    const auto f_cos = [Br,tau](const double omega){return Br(omega)*cos(omega*tau);};
    switch (type_of_quadrature_for_cos_transform)
    {
        case TypeOfQuadrature::GAUSS_KRONROD:
            return 2./PI*ssc::integrate::GaussKronrod(f_cos).integrate_f(a,b);
            break;
        case TypeOfQuadrature::RECTANGLE:
            return 2./PI*ssc::integrate::Rectangle(f_cos).integrate_f(a, b);
            break;
        case TypeOfQuadrature::SIMPSON:
            return 2./PI*ssc::integrate::Simpson(f_cos).integrate_f(a, b);
            break;
        case TypeOfQuadrature::TRAPEZOIDAL:
            return 2./PI*ssc::integrate::TrapezoidalIntegration(f_cos).integrate_f(a, b);
            break;
        case TypeOfQuadrature::BURCHER:
            return 2./PI*ssc::integrate::Burcher(Br,tau).integrate_f(a, b);
            break;
        case TypeOfQuadrature::CLENSHAW_CURTIS:
            return 2./PI*ssc::integrate::ClenshawCurtisCosine(Br,tau).integrate_f(a, b);
            break;
        case TypeOfQuadrature::FILON:
            return 2./PI*ssc::integrate::Filon(Br,tau).integrate_f(a, b);
            break;
        default:
            THROW(__PRETTY_FUNCTION__, DampingMatrixInterpolatorException, "Unknown type of quadrature.");
            break;
    }
    return 0;
}

std::function<double(double)> RadiationDampingBuilder::build_retardation_function(const std::function<double(double)>& Br, const std::vector<double>& taus, const double eps, const double omega_min, double omega_max) const
{
    omega_max = find_integration_bound(Br, omega_min, omega_max, eps);
    std::vector<double> y;
    for (auto tau:taus)
    {
        y.push_back(cos_transform(Br, omega_min, omega_max, tau));
    }
    return build_interpolator(taus, y);
}

double RadiationDampingBuilder::convolution(const History& h, //!< State history
                           const std::function<double(double)>& f, //!< Function to convolute with
                           const double Tmin, //!< Beginning of the convolution (because retardation function may not be defined for T=0)
                           const double Tmax  //!< End of the convolution
                           ) const
{
    const auto g = [&h, &f](const double tau){return h.get(tau)*f(tau);};
    return ssc::integrate::GaussKronrod(g).integrate_f(Tmin, Tmax);
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

std::vector<double> RadiationDampingBuilder::build_exponential_intervals(const double first, //!< First value in vector
                                                    const double last, //!< Last value in vector
                                                    const size_t n //!< Number of values to return
                                                    ) const
{
    std::vector<double> ret(n);
    const double sgn = first < last ? 1 : -1;
    const double L = std::abs(last-first);
    for (size_t i = 0 ; i < n ; ++i)
    {
        ret[i] = first + sgn*(std::pow(L+1,(double)i/((double)n-1))-1);
    }
    return ret;
}

std::vector<double> RadiationDampingBuilder::build_exponential_intervals_reversed(const double first, //!< First value in vector
                                                    const double last, //!< Last value in vector
                                                    const size_t n //!< Number of values to return
                                                    ) const
{
    std::vector<double> ret = build_exponential_intervals(last, first, n);
    std::reverse(ret.begin(),ret.end());
    return ret;
}

double RadiationDampingBuilder::find_integration_bound(const std::function<double(double)>& f, //!< Function to integrate
                                                       const double omega_min,                 //!< Lower bound of the integration (returned omega is necessarily greater than omega_min)
                                                       const double omega_max,                 //!< Upper bound of the integration (returned omega is necessarily lower than omega_min)
                                                       const double eps                        //!< Integration error (compared to full integration from omega_min up to omega_max)
                                                       ) const
{
    boost::math::tools::eps_tolerance<double> tol(30);
    if (std::abs(f(omega_max))>eps) return omega_max;
    if (std::abs(f(omega_min))<eps) return omega_min;
    const auto g = [&f,eps](const double omega){return std::abs(f(omega))-eps;};
    boost::uintmax_t max_iter=100;
    return boost::math::tools::toms748_solve(g, omega_min, omega_max, tol, max_iter).first;
}

double RadiationDampingBuilder::find_r_bound(const std::function<double(double)>& f, //!< Function to integrate
                                                       const double omega_min,                 //!< Lower bound of the integration (returned omega is necessarily greater than omega_min)
                                                       const double omega_max,                 //!< Upper bound of the integration (returned omega is necessarily lower than omega_min)
                                                       const double r //!< How much of the total integral between omega_min & omega_max do we wish to represent?
                                                       ) const
{
    boost::math::tools::eps_tolerance<double> tol(30);
    const double I0 = cos_transform(f, omega_min, omega_max, 0);
    const auto g = [&f,this,omega_min,I0,r](const double omega){return cos_transform(f,omega_min, omega, 0)-r*I0;};
    boost::uintmax_t max_iter=100;
    return boost::math::tools::toms748_solve(g, omega_min, omega_max, tol, max_iter).first;
}
