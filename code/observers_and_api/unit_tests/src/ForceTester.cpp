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
#include "GMForceModel.hpp"
#include "DiffractionForceModel.hpp"
#include "hdb_data.hpp"
#include "yaml_data.hpp"
#include "stl_data.hpp"

Sim ForceTester::make_sim(const std::string& yaml, const std::string& stl) const
{
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

    return get_system(check_input_yaml(input), stl, 0);
}

Sim ForceTester::make_sim(const std::string& yaml, const VectorOfVectorOfPoints& stl) const
{
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

    return get_system(check_input_yaml(input), stl, 0);
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

std::vector<double> get_states(const double x, const double y,
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
    const auto quaternion = BodyStates::convert(angle, c);
    states[QRIDX(0)] = std::get<0>(quaternion);
    states[QIIDX(0)] = std::get<1>(quaternion);
    states[QJIDX(0)] = std::get<2>(quaternion);
    states[QKIDX(0)] = std::get<3>(quaternion);
    return states;
}

std::vector<double> ForceTester::set_states(const double x, const double y,
        const double z, const double phi, const double theta, const double psi)
{
    const std::vector<double> states = get_states(x,y,z,phi,theta,psi);
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

double ForceTester::immersed_surface(const double x,
                                     const double y,
                                     const double z,
                                     const double phi,
                                     const double theta,
                                     const double psi)
{
    set_states(x, y, z, phi, theta, psi);
    double S = 0;
    for (auto it = body->get_states().intersector->begin_immersed() ; it != body->get_states().intersector->end_immersed() ; ++it)
    {
        S += it->area;
    }
    return S;
}

double ForceTester::emerged_surface(const double x,
                                    const double y,
                                    const double z,
                                    const double phi,
                                    const double theta,
                                    const double psi)
{
    set_states(x, y, z, phi, theta, psi);
    double S = 0;
    for (auto it = body->get_states().intersector->begin_emerged() ; it != body->get_states().intersector->end_emerged() ; ++it)
    {
        S += it->area;
    }
    return S;
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

EPoint ForceTester::center_of_buoyancy_in_ned_frame(const double x,
                                                    const double y,
                                                    const double z,
                                                    const double phi,
                                                    const double theta,
                                                    const double psi)
{
    std::vector<double> states = set_states(x, y, z, phi, theta, psi);
    auto Tned2body = body->get_transform_from_ned_to_body(states);
    Tned2body.swap();
    for (const auto force:forces)
    {
        const auto hs = dynamic_cast<HydrostaticForceModel*>(force.get());
        if (hs)
        {
            hs->operator()(body->get_states(), current_instant++);
            return (Tned2body*hs->get_centre_of_buoyancy()).v;
        }
    }
    return EPoint(std::nan(""),std::nan(""),std::nan(""));
}

boost::optional<double> ForceTester::gm(const double x,
                       const double y,
                       const double z,
                       const double phi,
                       const double theta,
                       const double psi)
{
    std::vector<double> states = set_states(x, y, z, phi, theta, psi);
    boost::optional<double> ret;
    for (auto force:forces)
    {
        if (force->get_name() == "GM")
        {
            GMForceModel* F = static_cast<GMForceModel*>(force.get());
            F->operator ()(body->get_states(), 0);
            ret = F->get_GM();
        }
    }
    return ret;
}
