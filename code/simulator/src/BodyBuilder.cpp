/*
 * BodyBuilder.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#include "BodyBuilder.hpp"
#include "YamlBody.hpp"

BodyBuilder::BodyBuilder(const YamlRotation& convention) : rotations(convention)
{
}

Body BodyBuilder::build(const YamlBody& input, const VectorOfVectorOfPoints& ) const
{
    Body ret;
    ret.name = input.name;
    return ret;
}
