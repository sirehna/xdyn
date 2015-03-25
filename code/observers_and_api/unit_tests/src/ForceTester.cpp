/*
 * ForceTester.cpp
 *
 *  Created on: Feb 16, 2015
 *      Author: cady
 */


#include "check_input_yaml.hpp"
#include "ForceTester.hpp"
#include "HydrostaticForceModel.hpp"
#include "SimulatorYamlParser.hpp"
#include "simulator_api.hpp"
#include "StateMacros.hpp"

Sim ForceTester::make_sim(const std::string& yaml, const std::string& stl) const
{
    ssc::data_source::DataSource command_listener;
    auto input = SimulatorYamlParser(yaml).parse();
    YamlBody body = input.bodies.front();
    body.controlled_forces.clear();
    body.external_forces.clear();
    YamlModel waves, hydrostatic;
    hydrostatic.model = "non-linear hydrostatic (exact)"; // So the builder creates a BodyWithSurfaceForces object
    body.external_forces.push_back(hydrostatic);

    input.bodies[0] = body;
    input.environment.clear();
    waves.model = "no waves";
    waves.yaml = "constant sea elevation in NED frame: {value: 0, unit: m}";
    input.environment.push_back(waves);

    return get_system(check_input_yaml(input), stl, 0, command_listener);
}

Sim ForceTester::make_sim(const std::string& yaml, const VectorOfVectorOfPoints& stl) const
{
    ssc::data_source::DataSource command_listener;
    auto input = SimulatorYamlParser(yaml).parse();
    YamlBody body = input.bodies.front();
    body.controlled_forces.clear();
    body.external_forces.clear();
    YamlModel waves, hydrostatic;
    hydrostatic.model = "non-linear hydrostatic (exact)"; // So the builder creates a BodyWithSurfaceForces object
    body.external_forces.push_back(hydrostatic);

    input.bodies[0] = body;
    input.environment.clear();
    waves.model = "no waves";
    waves.yaml = "constant sea elevation in NED frame: {value: 0, unit: m}";
    input.environment.push_back(waves);

    return get_system(check_input_yaml(input), stl, 0, command_listener);
}

ForceTester::ForceTester(const std::string& yaml, const VectorOfVectorOfPoints& stl) :
forces(),
body(),
env(),
current_instant(0)
{
    auto sys = make_sim(yaml, stl);
    body = sys.get_bodies().front();
    env = sys.get_env();
}

ForceTester::ForceTester(const std::string& yaml, const std::string& stl) :
        forces(),
        body(),
        env(),
        current_instant(0)
{
    auto sys = make_sim(yaml, stl);
    body = sys.get_bodies().front();
    env = sys.get_env();
}

std::vector<double> ForceTester::set_states(const double x, const double y,
        const double z, const double phi, const double theta, const double psi)
{
    std::vector<double> states(13, 0);
    states[XIDX(0)] = x;
    states[YIDX(0)] = y;
    states[ZIDX(0)] = z;
    ssc::kinematics::EulerAngles angle(phi,theta,psi);
    YamlRotation c;
    c.order_by = "angle";
    c.convention.push_back("z");
    c.convention.push_back("y'");
    c.convention.push_back("x''");
    const auto quaternion = body->get_quaternions(angle, c);
    states[QRIDX(0)] = std::get<0>(quaternion);
    states[QIIDX(0)] = std::get<1>(quaternion);
    states[QJIDX(0)] = std::get<2>(quaternion);
    states[QKIDX(0)] = std::get<3>(quaternion);
    body->update(env,states,current_instant);
    current_instant += 1;
    return states;
}

ssc::kinematics::Wrench ForceTester::force_in_ned(const double x,
                                                  const double y,
                                                  const double z,
                                                  const double phi,
                                                  const double theta,
                                                  const double psi)
{
    std::vector<double> states = set_states(x, y, z, phi, theta, psi);
    ssc::kinematics::Wrench ret;
    if (not(forces.empty())) ret = forces.front()->operator()(body->get_states(), current_instant);
    for (size_t i = 1 ; i < forces.size() ; ++i)
    {
        auto f = forces.at(i)->operator()(body->get_states(), current_instant);
        ret = ret + f;
    }

    ret.force = body->get_rot_from_ned_to(states)*ret.force;
    ret.torque = body->get_rot_from_ned_to(states)*ret.torque;
    return ret;
}

double ForceTester::immersed_volume(const double x,
                                    const double y,
                                    const double z,
                                    const double phi,
                                    const double theta,
                                    const double psi)
{
    set_states(x, y, z, phi, theta, psi);
    return body->get_states().intersector->immersed_volume();
}

double ForceTester::emerged_volume(const double x,
                                    const double y,
                                    const double z,
                                    const double phi,
                                    const double theta,
                                    const double psi)
{
    set_states(x, y, z, phi, theta, psi);
    return body->get_states().intersector->emerged_volume();
}
