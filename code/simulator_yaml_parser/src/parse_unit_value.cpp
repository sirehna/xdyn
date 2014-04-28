/*
 * parse_unit_value.cpp
 *
 *  Created on: Apr 28, 2014
 *      Author: cady
 */

#include "DecodeUnit.h"
#include "parse_unit_value.hpp"

void parse_uv(const YAML::Node& node, double& d)
{
    UV uv;
    node >> uv;
    d = decode(uv);
}

void operator >> (const YAML::Node& node, UV& g)
{
    node["unit"] >> g.unit;
    node["value"] >> g.value;
}

double decode(const UV& uv)
{
    return uv.value * DecodeUnit::decodeUnit(uv.unit);
}
