/*
 * parse_unit_value.cpp
 *
 *  Created on: Apr 28, 2014
 *      Author: cady
 */

#include <ssc/decode_unit.hpp>
#include "parse_unit_value.hpp"

void parse_uv(const YAML::Node& node, std::vector<double>& d)
{
    if(node.FindValue("unit"))
    {
        std::string unit = "";
        node["unit"] >> unit;
        const double factor = ssc::decode_unit::decodeUnit(unit);
        node["values"] >> d;
        for (std::vector<double>::iterator it = d.begin() ; it != d.end() ; ++it)
        {
            *it *= factor;
        }
    }
    else
    {
        node >> d;
    }
}

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
    return uv.value * ssc::decode_unit::decodeUnit(uv.unit);
}
