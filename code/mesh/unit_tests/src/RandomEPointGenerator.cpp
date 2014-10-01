/*
 * RandomEPointGenerator.cpp
 *
 *  Created on: Aug 22, 2014
 *      Author: cady
 */

#include <ssc/random_data_generator.hpp>
#include "GeometricTypes3d.hpp"

namespace ssc
{
    namespace random_data_generator
    {
        template <>
        EPoint TypedScalarDataGenerator<EPoint>::get() const
        {
            return EPoint(random<double>(),random<double>(),random<double>());
        }
    }
}


