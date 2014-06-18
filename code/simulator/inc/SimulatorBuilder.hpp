/*
 * SimulatorBuilder.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#ifndef SIMULATORBUILDER_HPP_
#define SIMULATORBUILDER_HPP_

#include <map>

#include "BodyBuilder.hpp"
#include "ForceBuilder.hpp"
#include "WaveBuilder.hpp"
#include "YamlSimulatorInput.hpp"

class EnvironmentAndFrames;

typedef std::map<std::string, VectorOfVectorOfPoints> MeshMap;

class SimulatorBuilder
{
    public:
        SimulatorBuilder(const YamlSimulatorInput& input);
        std::vector<Body> get_bodies(const MeshMap& meshes) const;

    private:
        SimulatorBuilder();

        YamlSimulatorInput input;
        BodyBuilder builder;
        std::vector<ForceBuilderPtr> force_parsers;
        std::vector<WaveBuilderPtr> wave_parsers;
};


#endif /* SIMULATORBUILDER_HPP_ */
