/*
 * BodyBuilder.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#include "BodyBuilder.hpp"

BodyBuilder::BodyBuilder(const YamlRotation& convention) : rotations(convention)
{
}

Body BodyBuilder::build(const YamlBody& , const VectorOfVectorOfPoints& ) const
{
    Body ret;
    return ret;
}
