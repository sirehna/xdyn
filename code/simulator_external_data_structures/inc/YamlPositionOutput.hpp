/*
 * YamlPositionOutput.hpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#ifndef YAMLPOSITIONOUTPUT_HPP_
#define YAMLPOSITIONOUTPUT_HPP_

#include "YamlOutput.hpp"

struct YamlPositionOutput : public YamlOutput
{
    YamlPositionOutput();
    YamlPositionOutput(const YamlOutput& rhs);
    std::string point;
    std::string relative_to_frame;
};

#endif /* YAMLPOSITIONOUTPUT_HPP_ */
