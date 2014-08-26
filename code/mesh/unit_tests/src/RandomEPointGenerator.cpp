/*
 * RandomEPointGenerator.cpp
 *
 *  Created on: Aug 22, 2014
 *      Author: cady
 */

#include "DataGenerator.hpp"
#include "GeometricTypes3d.hpp"

template <>
EPoint TypedScalarDataGenerator<EPoint>::get() const
{
    return EPoint(random<double>(),random<double>(),random<double>());
}


