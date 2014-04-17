/*
 * YamlPositionOutput.cpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#include "YamlPositionOutput.hpp"

YamlPositionOutput::YamlPositionOutput() : YamlOutput(), point(""), relative_to_frame("")
{}

YamlPositionOutput::YamlPositionOutput(const YamlOutput& rhs) : YamlOutput(rhs), point(""), relative_to_frame("")
{

}
