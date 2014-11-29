/*
 * DampingMatrixInterpolator.cpp
 *
 *  Created on: Nov 27, 2014
 *      Author: cady
 */

#include <ssc/macros/tr1_macros.hpp>
#include TR1INC(memory)

#include <ssc/interpolation.hpp>
typedef TR1(shared_ptr)<ssc::interpolation::Interpolator> InterpolatorPtr;

#include "DampingMatrixInterpolator.hpp"
#include "DampingMatrixInterpolatorException.hpp"

DampingMatrixInterpolator::DampingMatrixInterpolator()
{
}

std::function<double(double)> DampingMatrixInterpolator::build_interpolator(const std::vector<double>& x, const std::vector<double>& y, const TypeOfInterpolation& type) const
{
    InterpolatorPtr i;
    switch(type)
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
