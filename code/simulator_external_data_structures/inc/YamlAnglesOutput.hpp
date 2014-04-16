/*
 * YamlAnglesOutput.hpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#ifndef YAMLANGLESOUTPUT_HPP_
#define YAMLANGLESOUTPUT_HPP_

#include "YamlOutput.hpp"

struct YamlAnglesOutput : public YamlOutput
{
    YamlAnglesOutput();
    std::string frame;
    std::string relative_to_frame;
};

#endif /* YAMLANGLESOUTPUT_HPP_ */
