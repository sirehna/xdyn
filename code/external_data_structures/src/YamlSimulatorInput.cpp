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
                                           points(std::vector<YamlPoint>()),
                                           position_output(std::vector<YamlPositionOutput>()),
                                           angles_output(std::vector<YamlAnglesOutput>()),
                                           linear_velocities_output(std::vector<YamlPositionOutput>()),
                                           angular_velocities_output(std::vector<YamlAnglesOutput>()),
                                           linear_accelerations_output(std::vector<YamlPositionOutput>()),
                                           angular_accelerations_output(std::vector<YamlAnglesOutput>()),
                                           forces_and_torques_output(std::vector<YamlForcesAndTorquesOutput>())
{
}
