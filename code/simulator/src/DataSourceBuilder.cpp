/*
 * DataSourceBuilder.cpp
 *
 *  Created on: 18 avr. 2014
 *      Author: cady
 */

#include "DataSourceBuilder.hpp"
#include "EulerAngles.hpp"
#include "KinematicsModule.hpp"
#include "GravityModule.hpp"
#include "force_parsers.hpp"
#include "TriMeshBuilder.hpp"
#include "TextFileReader.hpp"
#include "StlReader.hpp"

#include <Eigen/Geometry>

#include <functional> // std::bind1st
#include <boost/bind.hpp>

#include "rotation_matrix_builders.hpp"
using namespace kinematics;

#define FOR_EACH(v,f)  std::for_each(v.begin(),\
			                         v.end(),\
			                         boost::bind(&DataSourceBuilder::f, boost::ref(*this), _1));

MODULE(PointMatrixBuilder, const std::string name = get_name();\
                           const TriMesh T = ds->get<TriMesh>(name);\
                           ds->set<PointMatrix>(name, PointMatrix(T.nodes,name));
      )

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

void DataSourceBuilder::add_initial_quaternions(const YamlBody& body)
{
	const YamlAngle angle = body.initial_position_of_body_frame_relative_to_NED_projected_in_NED.angle;
	const EulerAngles a(angle.phi, angle.theta, angle.psi);
	const RotationMatrix R = rotation_matrix<INTRINSIC, ORDERED_BY_ANGLE, CARDAN, 3, 2, 1>(a);
	const Eigen::Quaternion<double> q(R);
	ds.set(std::string("qr(")+body.name+")", q.w());
	ds.set(std::string("qi(")+body.name+")", q.x());
	ds.set(std::string("qj(")+body.name+")", q.y());
	ds.set(std::string("qk(")+body.name+")", q.z());
}

void DataSourceBuilder::add_states(const YamlBody& body)
{
	ds.define_derivative(std::string("x(")+body.name+")", std::string("dx/dt(")+body.name+")");
	ds.define_derivative(std::string("y(")+body.name+")", std::string("dy/dt(")+body.name+")");
	ds.define_derivative(std::string("z(")+body.name+")", std::string("dz/dt(")+body.name+")");
	ds.define_derivative(std::string("u(")+body.name+")", std::string("du/dt(")+body.name+")");
	ds.define_derivative(std::string("v(")+body.name+")", std::string("dv/dt(")+body.name+")");
	ds.define_derivative(std::string("w(")+body.name+")", std::string("dw/dt(")+body.name+")");
	ds.define_derivative(std::string("p(")+body.name+")", std::string("dp/dt(")+body.name+")");
	ds.define_derivative(std::string("q(")+body.name+")", std::string("dq/dt(")+body.name+")");
	ds.define_derivative(std::string("r(")+body.name+")", std::string("dr/dt(")+body.name+")");
	ds.define_derivative(std::string("qr(")+body.name+")", std::string("dqr/dt(")+body.name+")");
	ds.define_derivative(std::string("qi(")+body.name+")", std::string("dqi/dt(")+body.name+")");
	ds.define_derivative(std::string("qj(")+body.name+")", std::string("dqj/dt(")+body.name+")");
	ds.define_derivative(std::string("qk(")+body.name+")", std::string("dqk/dt(")+body.name+")");
}

void DataSourceBuilder::add_forces(const YamlBody& body)
{
    std::vector<YamlModel>::const_iterator that_model = body.external_forces.begin();
    for (;that_model!=body.external_forces.end();++that_model)
    {
        if (that_model->model == "gravity") add_gravity(body.name, that_model->yaml, body.dynamics.mass);
    }
}

void DataSourceBuilder::add_gravity(const std::string& body_name, const std::string& yaml, const double mass)
{
    GravityModule g(&ds, "gravity", body_name);
    ds.add(g);
    ds.set<double>(std::string("m(") + body_name + ")", mass);
    ds.set<double>("g", parse_gravity(yaml).g);
}

void DataSourceBuilder::add_mesh(const YamlBody& body)
{
    const TextFileReader reader(std::vector<std::string>(1, body.mesh));
    const VectorOfVectorOfPoints data = read_stl(reader.get_contents());
    TriMeshBuilder builder(data);
    ds.set<TriMesh>(body.name, builder.build());
    ds.add<PointMatrixBuilder>(body.name);
}

void DataSourceBuilder::add_kinematics(const std::vector<YamlBody>& bodies)
{
    std::vector<std::string> body_names;
    for (size_t i = 0 ; i < bodies.size() ; ++i)
    {
        body_names.push_back(bodies[i].name);
    }

    ds.add(KinematicsModule(&ds, body_names, "kinematics"));
}

DataSource DataSourceBuilder::build_ds()
{
	FOR_EACH(input.bodies, add_initial_conditions);
	FOR_EACH(input.bodies, add_initial_quaternions);
	FOR_EACH(input.bodies, add_states);
	FOR_EACH(input.bodies, add_forces);
	FOR_EACH(input.bodies, add_mesh);

	add_kinematics(input.bodies);

    return ds;
}
