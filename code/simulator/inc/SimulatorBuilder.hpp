/*
 * SimulatorBuilder.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#ifndef SIMULATORBUILDER_HPP_
#define SIMULATORBUILDER_HPP_

#include <map>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>

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

        template <typename T> SimulatorBuilder& can_parse(typename boost::enable_if<boost::is_base_of<WaveModelInterface,T> >::type* dummy = 0)
        {
            (void)dummy;
            wave_parsers.push_back(WaveBuilderPtr(new WaveBuilder<T>()));
            return *this;
        }

        std::vector<Body> get_bodies(const MeshMap& meshes) const;
        EnvironmentAndFrames get_environment_and_frames(const std::vector<Body>& bodies) const;

    private:
        SimulatorBuilder();
        WavePtr get_wave() const;

        YamlSimulatorInput input;
        BodyBuilder builder;
        std::vector<ForceBuilderPtr> force_parsers;
        std::vector<WaveBuilderPtr> wave_parsers;
};


#endif /* SIMULATORBUILDER_HPP_ */
