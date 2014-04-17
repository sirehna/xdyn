/*
 * YamlForcesAndTorquesOutput.hpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#ifndef YAMLFORCESANDTORQUESOUTPUT_HPP_
#define YAMLFORCESANDTORQUESOUTPUT_HPP_

#include "YamlOutput.hpp"

struct YamlForcesAndTorquesOutput : public YamlOutput
{
    YamlForcesAndTorquesOutput();
    std::string type;
    std::string point_name;
    std::string point_frame;
};

#endif /* YAMLFORCESANDTORQUESOUTPUT_HPP_ */
