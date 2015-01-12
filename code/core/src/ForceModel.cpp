/*
 * ForceModel.cpp
 *
 *  Created on: Oct 27, 2014
 *      Author: cady
 */

#include "ForceModel.hpp"

ForceModel::ForceModel(const std::string& name_) : name(name_), force()
{
}

bool ForceModel::is_a_surface_force_model() const
{
    return false;
}

std::string ForceModel::get_name() const
{
    return name;
}

void ForceModel::update(const BodyStates& body, const double t)
{
    force = this->operator()(body, t);
}

ssc::kinematics::Wrench ForceModel::get() const
{
    return force;
}
