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
    std::string address;
    short unsigned int port;
    std::vector<std::string> data;
};

#endif /* YAMLOUTPUT_HPP_ */
