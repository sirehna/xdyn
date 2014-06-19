/*
 * SimulatorBuilder.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include "Kinematics.hpp"
#include "SimulatorBuilder.hpp"
#include "SimulatorBuilderException.hpp"
#include "update_kinematics.hpp"

SimulatorBuilder::SimulatorBuilder(const YamlSimulatorInput& input_) :
                                        input(input_),
                                        builder(BodyBuilder(input.rotations)),
                                        force_parsers(std::vector<ForceBuilderPtr>()),
                                        wave_parsers(std::vector<WaveBuilderPtr>())
{
}

std::vector<Body> SimulatorBuilder::get_bodies(const MeshMap& meshes) const
{
    std::vector<Body> ret;
    for (const auto body:input.bodies)
    {
        const auto that_mesh = meshes.find(body.name);
        if (that_mesh != meshes.end())
        {
            ret.push_back(builder.build(body, that_mesh->second));
        }
        else
        {
            THROW(__PRETTY_FUNCTION__, SimulatorBuilderException, std::string("Unable to find mesh for '") + body.name + "' in map");
        }
    }
    return ret;
}

EnvironmentAndFrames SimulatorBuilder::get_environment_and_frames(const std::vector<Body>& bodies) const
{
    EnvironmentAndFrames env;
    env.g = input.environmental_constants.g;
    env.rho = input.environmental_constants.rho;
    env.k = KinematicsPtr(new Kinematics());
    const StateType x = get_initial_states(input.rotations, input.bodies);
    for (size_t i = 0 ; i < bodies.size() ; ++i)
    {
        env.k->add(get_transform_from_mesh_to(bodies.at(i)));
        env.k->add(get_transform_from_ned_to(x, bodies.at(i), i));
    }
    return env;
}
