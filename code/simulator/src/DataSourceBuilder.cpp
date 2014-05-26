/*
 * DataSourceBuilder.cpp
 *
 *  Created on: 18 avr. 2014
 *      Author: cady
 */

#include "DataSourceBuilder.hpp"
#include "DataSourceBuilderException.hpp"
#include "EulerAngles.hpp"
#include "KinematicsModule.hpp"
#include "GravityModule.hpp"
#include "force_parsers.hpp"
#include "MeshBuilder.hpp"
#include "TextFileReader.hpp"
#include "StlReader.hpp"
#include "WaveModule.hpp"
#include "environment_parsers.hpp"
#include "DefaultWaveModel.hpp"

#include <Eigen/Geometry>

#include <functional> // std::bind1st
#include <boost/bind.hpp>

#include "rotation_matrix_builders.hpp"
using namespace kinematics;

#define FOR_EACH(v,f)  std::for_each(v.begin(),\
			                         v.end(),\
			                         boost::bind(&DataSourceBuilder::f, boost::ref(*this), _1));

MODULE(PointMatrixBuilder, const std::string name = get_name();\
                           const std::tr1::shared_ptr<Mesh> T = ds->get<std::tr1::shared_ptr<Mesh> >(name);\
                           const std::tr1::shared_ptr<PointMatrix> pm = ds->read_only() ? std::tr1::shared_ptr<PointMatrix>() : std::tr1::shared_ptr<PointMatrix>(new PointMatrix(T->nodes,name));\
                           ds->set<std::tr1::shared_ptr<PointMatrix> >(name, pm);
      )

DataSourceBuilder::DataSourceBuilder(const YamlSimulatorInput& in) : input(in), ds(DataSource()), rotations(in.rotations)
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
	ds.set(std::string("x(")+body.name+"/mesh)", body.position_of_body_frame_relative_to_mesh.coordinates.x);
	ds.set(std::string("y(")+body.name+"/mesh)", body.position_of_body_frame_relative_to_mesh.coordinates.y);
	ds.set(std::string("z(")+body.name+"/mesh)", body.position_of_body_frame_relative_to_mesh.coordinates.z);
	const EulerAngles a(body.position_of_body_frame_relative_to_mesh.angle.phi, body.position_of_body_frame_relative_to_mesh.angle.theta, body.position_of_body_frame_relative_to_mesh.angle.psi);
	ds.set(body.name+"/mesh", angle2matrix(a));
}

bool match(const std::vector<std::string>& convention, const std::string& first, const std::string& second, const std::string& third);
bool match(const std::vector<std::string>& convention, const std::string& first, const std::string& second, const std::string& third)
{
    return (convention.at(0) == first) and (convention.at(1) == second) and (convention.at(2) == third);
}

RotationMatrix DataSourceBuilder::angle2matrix(const EulerAngles& a) const
{
    if (rotations.order_by == "angle")
    {
        if (match(rotations.convention, "x", "y'", "z''"))
            return rotation_matrix<INTRINSIC, ORDERED_BY_ANGLE, CARDAN, 3, 2, 1>(a);
        std::stringstream ss;
        ss << "Rotation convention '" << rotations.convention.at(0) << "," << rotations.convention.at(1) << "," << rotations.convention.at(2) << "' is not currently supported.";
        THROW(__PRETTY_FUNCTION__, DataSourceBuilderException, ss.str());
    }
    else
    {
        THROW(__PRETTY_FUNCTION__, DataSourceBuilderException, std::string("Ordering rotations by '") + rotations.order_by + "' is not currently supported");
    }
    return RotationMatrix();
}

void DataSourceBuilder::add_initial_quaternions(const YamlBody& body)
{
	const YamlAngle angle = body.initial_position_of_body_frame_relative_to_NED_projected_in_NED.angle;
	const EulerAngles a(angle.phi, angle.theta, angle.psi);
	const RotationMatrix R = angle2matrix(a);
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
    MeshBuilder builder(read_stl(reader.get_contents()));
    std::tr1::shared_ptr<Mesh> mesh(new Mesh(builder.build()));
    ds.set<std::tr1::shared_ptr<Mesh> >(body.name, mesh);
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

void DataSourceBuilder::add_centre_of_gravity(const YamlBody& body)
{
    const Point G(body.dynamics.centre_of_inertia.frame,
                  body.dynamics.centre_of_inertia.x,
                  body.dynamics.centre_of_inertia.y,
                  body.dynamics.centre_of_inertia.z);
    ds.set(std::string("G(")+body.name+")", G);
}

void DataSourceBuilder::add_wave_height_module(const YamlBody& body)
{
    ds.add(WaveModule(&ds, std::string("wave module(") + body.name + ")", body.name));
}

void DataSourceBuilder::add_default_wave_model(const std::string& yaml)
{
    const double zwave = parse_default_wave_model(yaml);
    ds.set<std::tr1::shared_ptr<WaveModelInterface> >("wave model", std::tr1::shared_ptr<WaveModelInterface>(new DefaultWaveModel(zwave)));
}

void DataSourceBuilder::add_environment_model(const YamlModel& model)
{
    if (model.model=="no waves") add_default_wave_model(model.yaml);
}

DataSource DataSourceBuilder::build_ds()
{
    FOR_EACH(input.bodies, add_initial_conditions);
    FOR_EACH(input.bodies, add_initial_quaternions);
    FOR_EACH(input.bodies, add_states);
    FOR_EACH(input.bodies, add_forces);
    FOR_EACH(input.bodies, add_centre_of_gravity);
    FOR_EACH(input.environment, add_environment_model);
    FOR_EACH(input.bodies, add_mesh);
    FOR_EACH(input.bodies, add_wave_height_module);

	add_kinematics(input.bodies);

    return ds;
}
