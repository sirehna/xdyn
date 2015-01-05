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
#include "YamlBlockedDegreesOfFreedom.hpp"
#include "YamlPositionOutput.hpp"
#include "YamlAnglesOutput.hpp"
#include "YamlForcesAndTorquesOutput.hpp"
#include "YamlEnvironmentalConstants.hpp"

struct YamlSimulatorInput
{
    YamlSimulatorInput();
    YamlRotation rotations;
    YamlEnvironmentalConstants environmental_constants;
    std::vector<YamlModel> environment;
    std::vector<YamlBody> bodies;
    std::vector<YamlPoint> points;
    std::vector<YamlBlockedDegreesOfFreedom> blocked_degrees_of_freedom;
    std::vector<YamlPositionOutput> position_output;
    std::vector<YamlAnglesOutput> angles_output;
    std::vector<YamlPositionOutput> linear_velocities_output;
    std::vector<YamlAnglesOutput> angular_velocities_output;
    std::vector<YamlPositionOutput> linear_accelerations_output;
    std::vector<YamlAnglesOutput> angular_accelerations_output;
    std::vector<YamlForcesAndTorquesOutput> forces_and_torques_output;
};

#endif /* YAMLSIMULATORINPUT_HPP_ */
