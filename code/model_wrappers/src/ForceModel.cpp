/*
 * ForceModel.cpp
 *
 *  Created on: Oct 27, 2014
 *      Author: cady
 */

#include "ForceModel.hpp"

ForceModel::ForceModel(const std::string& name_) : name(name_)
{
}

std::string ForceModel::get_name() const
{
    return name;
}
