/*
 * ForceModel.cpp
 *
 *  Created on: Oct 27, 2014
 *      Author: cady
 */

#include "ForceModel.hpp"
#include "Observer.hpp"
#include "BodyStates.hpp"

ForceModel::ForceModel(const std::string& force_name_, const std::string& body_name_) : force_name(force_name_), body_name(body_name_), force()
{
}

bool ForceModel::is_a_surface_force_model() const
{
    return false;
}

std::string ForceModel::get_name() const
{
    return force_name;
}

void ForceModel::update(const BodyStates& body, const double t)
{
    force = this->operator()(body, t);
    body_name = body.name;
}

ssc::kinematics::Wrench ForceModel::get() const
{
    return force;
}

void ForceModel::feed(Observer& observer) const
{
    std::vector<std::string> position(1,"efforts");
    observer.write(force.X(),DataAddressing(position,std::vector<std::string>{body_name,force_name,"Fx"},std::string("Fx(")+force_name+","+body_name+")"));
    observer.write(force.Y(),DataAddressing(position,std::vector<std::string>{body_name,force_name,"Fy"},std::string("Fy(")+force_name+","+body_name+")"));
    observer.write(force.Z(),DataAddressing(position,std::vector<std::string>{body_name,force_name,"Fz"},std::string("Fz(")+force_name+","+body_name+")"));
    observer.write(force.K(),DataAddressing(position,std::vector<std::string>{body_name,force_name,"Mx"},std::string("Mx(")+force_name+","+body_name+")"));
    observer.write(force.M(),DataAddressing(position,std::vector<std::string>{body_name,force_name,"My"},std::string("My(")+force_name+","+body_name+")"));
    observer.write(force.N(),DataAddressing(position,std::vector<std::string>{body_name,force_name,"Mz"},std::string("Mz(")+force_name+","+body_name+")"));
    extra_observations(observer);
}

void ForceModel::extra_observations(Observer& ) const
{
}
