/*
 * YamlAnglesOutput.cpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#include "YamlAnglesOutput.hpp"

YamlAnglesOutput::YamlAnglesOutput() : YamlOutput(), frame(""), relative_to_frame("")
{

}

YamlAnglesOutput::YamlAnglesOutput(const YamlOutput& rhs) : YamlOutput(rhs), frame(""), relative_to_frame("")
{

}
