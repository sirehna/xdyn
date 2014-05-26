/*
 * KinematicsModule.cpp
 *
 *  Created on: Apr 25, 2014
 *      Author: cady
 */

#include "KinematicsModule.hpp"
#include "Transform.hpp"
#include "Kinematics.hpp"
#include "DataSource.hpp"

using namespace kinematics;

KinematicsModule::KinematicsModule(DataSource* const data_source, const std::vector<std::string>& bodies_, const std::string& module_name) :
        DataSourceModule(data_source, module_name),
        bodies(bodies_),
        kinematics(std::tr1::shared_ptr<Kinematics>(new Kinematics()))
{
}

KinematicsModule::KinematicsModule(const KinematicsModule& rhs, DataSource* const data_source) :
	    DataSourceModule(rhs, data_source),
	    bodies(rhs.bodies),
	    kinematics(rhs.kinematics)
{
}

DataSourceModule* KinematicsModule::clone() const
{
	return new KinematicsModule(*this);
}

DataSourceModule* KinematicsModule::clone(DataSource* const data_source) const
{
	return new KinematicsModule(*this, data_source);
}

void KinematicsModule::update() const
{
	std::vector<std::string>::const_iterator that_body = bodies.begin();
	for (;that_body!=bodies.end() ; ++that_body)
	{
		kinematics->add(get_transform_from_ned_to(*that_body));
		kinematics->add(get_transform_from_mesh_to(*that_body));
	}
	ds->set("kinematics", kinematics);
}

Point KinematicsModule::get_origin(const std::string& body) const
{
	return Point("NED", ds->get<double>(std::string("x(")+body+")"),
                        ds->get<double>(std::string("y(")+body+")"),
                        ds->get<double>(std::string("z(")+body+")"));
}

RotationMatrix KinematicsModule::get_rot_from_ned_to(const std::string& body) const
{
	const Eigen::Quaternion<double> q(ds->get<double>(std::string("qr(")+body+")"),
				                      ds->get<double>(std::string("qi(")+body+")"),
				                      ds->get<double>(std::string("qj(")+body+")"),
				                      ds->get<double>(std::string("qk(")+body+")")
				                      );
	return q.matrix();
}

kinematics::Transform KinematicsModule::get_transform_from_mesh_to(const std::string& body) const
{
    return kinematics::Transform(get_position_of_body_relative_to_mesh(body), ds->get<RotationMatrix>(body + "/mesh"), body);
}

Point KinematicsModule::get_position_of_body_relative_to_mesh(const std::string& body) const
{
    return Point(std::string("mesh(")+body+")", ds->get<double>(std::string("x(")+body+"/mesh)"),
                                                ds->get<double>(std::string("y(")+body+"/mesh)"),
                                                ds->get<double>(std::string("z(")+body+"/mesh)"));
}

kinematics::Transform KinematicsModule::get_transform_from_ned_to(const std::string& body) const
{
	return kinematics::Transform(get_origin(body), get_rot_from_ned_to(body), body);
}
