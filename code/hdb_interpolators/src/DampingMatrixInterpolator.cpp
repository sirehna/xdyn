/*
 * DampingMatrixInterpolator.cpp
 *
 *  Created on: Nov 27, 2014
 *      Author: cady
 */

#include "DampingMatrixInterpolator.hpp"
#include "DampingMatrixInterpolatorException.hpp"

DampingMatrixInterpolator::DampingMatrixInterpolator()
{
}

InterpolatorPtr DampingMatrixInterpolator::build_interpolator(const std::vector<double>& x, const std::vector<double>& y, const TypeOfInterpolation& type) const
{
    InterpolatorPtr ret;
    switch(type)
    {
        case TypeOfInterpolation::LINEAR:
            ret.reset(new ssc::interpolation::LinearInterpolationVariableStep(x, y));
            break;
        case TypeOfInterpolation::PIECEWISE_CONSTANT:
            ret.reset(new ssc::interpolation::PiecewiseConstantVariableStep<double>(x, y));
            break;
        case TypeOfInterpolation::SPLINES:
            ret.reset(new ssc::interpolation::SplineVariableStep(x, y));
            break;
        default:
            THROW(__PRETTY_FUNCTION__, DampingMatrixInterpolatorException, "Unknown type of interpolation.");
            break;
    }
    return ret;
}
