/*
 * DataSourceBuilder.cpp
 *
 *  Created on: 18 avr. 2014
 *      Author: cady
 */

#include "DataSourceBuilder.hpp"
#include <functional> // std::bind1st
#include <boost/bind.hpp>

#define FOR_EACH(v,f)  std::for_each(v.begin(),\
			                         v.end(),\
			                         boost::bind(&DataSourceBuilder::f, boost::ref(*this), _1));

DataSourceBuilder::DataSourceBuilder(const YamlSimulatorInput& in) : input(in), ds(DataSource())
{
}

void DataSourceBuilder::add_initial_conditions(const YamlBody& body)
{
	ds.set(std::string("x(")+body.name+")", body.initial_position_of_body_frame_relative_to_NED_projected_in_NED.coordinates.x);
	ds.set(std::string("y(")+body.name+")", body.initial_position_of_body_frame_relative_to_NED_projected_in_NED.coordinates.y);
	ds.set(std::string("z(")+body.name+")", body.initial_position_of_body_frame_relative_to_NED_projected_in_NED.coordinates.z);
	ds.set(std::string("u(")+body.name+")", body.initial_velocity_of_body_frame_relative_to_NED_projected_in_body.u);
	ds.set(std::string("v(")+body.name+")", body.initial_velocity_of_body_frame_relative_to_NED_projected_in_body.v);
	ds.set(std::string("w(")+body.name+")", body.initial_velocity_of_body_frame_relative_to_NED_projected_in_body.w);
	ds.set(std::string("p(")+body.name+")", body.initial_velocity_of_body_frame_relative_to_NED_projected_in_body.p);
	ds.set(std::string("q(")+body.name+")", body.initial_velocity_of_body_frame_relative_to_NED_projected_in_body.q);
	ds.set(std::string("r(")+body.name+")", body.initial_velocity_of_body_frame_relative_to_NED_projected_in_body.r);
}

DataSource DataSourceBuilder::build_ds()
{
	FOR_EACH(input.bodies, add_initial_conditions);
    return ds;
}
