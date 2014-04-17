/*
 * YamlOutput.hpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#ifndef YAMLOUTPUT_HPP_
#define YAMLOUTPUT_HPP_

#include <string>
#include <vector>

struct YamlOutput
{
    YamlOutput();
    virtual ~YamlOutput(){}
    std::string projected_in_frame;
    std::vector<std::string> axes;
};

#endif /* YAMLOUTPUT_HPP_ */
