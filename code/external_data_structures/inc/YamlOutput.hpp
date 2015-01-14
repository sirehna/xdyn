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
    std::string filename;
    std::string format;
    std::vector<std::string> data;
};

#endif /* YAMLOUTPUT_HPP_ */
