/*
 * YamlSimulatorInput.cpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#include "YamlSimulatorInput.hpp"

YamlSimulatorInput::YamlSimulatorInput() : rotations(YamlRotation()),
                                           environmental_constants(YamlEnvironmentalConstants()),
                                           environment(std::vector<YamlModel>()),
                                           bodies(std::vector<YamlBody>()),
                                           points(std::vector<YamlPoint>())
{
}
