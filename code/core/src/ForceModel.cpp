/*
 * ForceModel.cpp
 *
 *  Created on: Oct 27, 2014
 *      Author: cady
 */

#include "ForceModel.hpp"
#include "Observer.hpp"

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

void ForceModel::feed(Observer& observer) const
{
    std::vector<std::string> position(1,name);
    observer.write(force.X(),position,std::string("Fx(")+force.get_frame()+")");
    observer.write(force.Y(),position,std::string("Fy(")+force.get_frame()+")");
    observer.write(force.Z(),position,std::string("Fz(")+force.get_frame()+")");
    observer.write(force.K(),position,std::string("Mx(")+force.get_frame()+")");
    observer.write(force.M(),position,std::string("My(")+force.get_frame()+")");
    observer.write(force.N(),position,std::string("Mz(")+force.get_frame()+")");
    extra_observations(observer);
}

void ForceModel::extra_observations(Observer& ) const
{
}
