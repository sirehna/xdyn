/*
 * XdynForME.cpp
 *
 *  Created on: Nov 13, 2018
 *      Author: cady
 */




#include "XdynForME.hpp"

XdynForME::XdynForME(const std::string& yaml_model) :builder(yaml_model)
, parser(builder.Tmax)
{

}

std::string XdynForME::calculate_dx_dt(const std::string& raw_yaml) const
{
    return raw_yaml;
}
