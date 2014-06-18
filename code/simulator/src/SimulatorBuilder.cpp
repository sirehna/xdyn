/*
 * SimulatorBuilder.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include "SimulatorBuilder.hpp"

SimulatorBuilder::SimulatorBuilder(const YamlSimulatorInput& input_) :
                                        input(input_),
                                        builder(BodyBuilder(input.rotations)),
                                        force_parsers(std::vector<ForceBuilderPtr>()),
                                        wave_parsers(std::vector<WaveBuilderPtr>())
{
}
