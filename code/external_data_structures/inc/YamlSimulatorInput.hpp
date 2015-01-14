/*
 * YamlSimulatorInput.hpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#ifndef YAMLSIMULATORINPUT_HPP_
#define YAMLSIMULATORINPUT_HPP_

#include "YamlRotation.hpp"
#include "YamlModel.hpp"
#include "YamlBody.hpp"
#include "YamlPoint.hpp"
#include "YamlEnvironmentalConstants.hpp"

struct YamlSimulatorInput
{
    YamlSimulatorInput();
    YamlRotation rotations;
    YamlEnvironmentalConstants environmental_constants;
    std::vector<YamlModel> environment;
    std::vector<YamlBody> bodies;
};

#endif /* YAMLSIMULATORINPUT_HPP_ */
