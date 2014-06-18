/*
 * SimulatorBuilder.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include "SimulatorBuilder.hpp"
#include "SimulatorBuilderException.hpp"

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
