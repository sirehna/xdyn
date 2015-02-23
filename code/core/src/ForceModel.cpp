/*
 * ForceModel.cpp
 *
 *  Created on: Oct 27, 2014
 *      Author: cady
 */

#include "ForceModel.hpp"
#include "Observer.hpp"
#include "BodyStates.hpp"

ForceModel::ForceModel(const std::string& force_name_, const std::string& body_name_) :
    force_name(force_name_),
    body_name(body_name_),
    force_in_body_frame(),
    force_in_ned_frame()
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
    force_in_body_frame = this->operator()(body, t);
    force_in_ned_frame = project_into_NED_frame(force_in_body_frame, body.get_rot_from_ned_to_body());
    body_name = body.name;
}

ssc::kinematics::Wrench ForceModel::project_into_NED_frame(const ssc::kinematics::Wrench& F, const ssc::kinematics::RotationMatrix& R)
{
    return ssc::kinematics::Wrench(ssc::kinematics::Point("NED"),R*F.force,R*F.torque);
}

ssc::kinematics::Wrench ForceModel::get_force_in_body_frame() const
{
    return force_in_body_frame;
}

ssc::kinematics::Wrench ForceModel::get_force_in_ned_frame() const
{
    return force_in_ned_frame;
}

void ForceModel::feed(Observer& observer) const
{
    observer.write(force_in_body_frame.X(),DataAddressing(std::vector<std::string>{"efforts",body_name,force_name,body_name,"Fx"},std::string("Fx(")+force_name+","+body_name+","+body_name+")"));
    observer.write(force_in_body_frame.Y(),DataAddressing(std::vector<std::string>{"efforts",body_name,force_name,body_name,"Fy"},std::string("Fy(")+force_name+","+body_name+","+body_name+")"));
    observer.write(force_in_body_frame.Z(),DataAddressing(std::vector<std::string>{"efforts",body_name,force_name,body_name,"Fz"},std::string("Fz(")+force_name+","+body_name+","+body_name+")"));
    observer.write(force_in_body_frame.K(),DataAddressing(std::vector<std::string>{"efforts",body_name,force_name,body_name,"Mx"},std::string("Mx(")+force_name+","+body_name+","+body_name+")"));
    observer.write(force_in_body_frame.M(),DataAddressing(std::vector<std::string>{"efforts",body_name,force_name,body_name,"My"},std::string("My(")+force_name+","+body_name+","+body_name+")"));
    observer.write(force_in_body_frame.N(),DataAddressing(std::vector<std::string>{"efforts",body_name,force_name,body_name,"Mz"},std::string("Mz(")+force_name+","+body_name+","+body_name+")"));

    observer.write(force_in_ned_frame.X(),DataAddressing(std::vector<std::string>{"efforts",body_name,force_name,"NED","Fx"},std::string("Fx(")+force_name+","+body_name+",NED)"));
    observer.write(force_in_ned_frame.Y(),DataAddressing(std::vector<std::string>{"efforts",body_name,force_name,"NED","Fy"},std::string("Fy(")+force_name+","+body_name+",NED)"));
    observer.write(force_in_ned_frame.Z(),DataAddressing(std::vector<std::string>{"efforts",body_name,force_name,"NED","Fz"},std::string("Fz(")+force_name+","+body_name+",NED)"));
    observer.write(force_in_ned_frame.K(),DataAddressing(std::vector<std::string>{"efforts",body_name,force_name,"NED","Mx"},std::string("Mx(")+force_name+","+body_name+",NED)"));
    observer.write(force_in_ned_frame.M(),DataAddressing(std::vector<std::string>{"efforts",body_name,force_name,"NED","My"},std::string("My(")+force_name+","+body_name+",NED)"));
    observer.write(force_in_ned_frame.N(),DataAddressing(std::vector<std::string>{"efforts",body_name,force_name,"NED","Mz"},std::string("Mz(")+force_name+","+body_name+",NED)"));
    extra_observations(observer);
}

void ForceModel::extra_observations(Observer& ) const
{
}
