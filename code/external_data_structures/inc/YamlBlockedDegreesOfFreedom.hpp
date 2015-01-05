/*
 * YamlBlockedDegreesOfFreedom.hpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#ifndef YAMLBLOCKEDDEGREESOFFREEDOM_HPP_
#define YAMLBLOCKEDDEGREESOFFREEDOM_HPP_

#include <string>
#include <vector>

struct YamlBlockedDegreesOfFreedom
{
    YamlBlockedDegreesOfFreedom();
    std::string body;
    std::vector<std::string> blocked;
};

#endif /* YAMLBLOCKEDDEGREESOFFREEDOM_HPP_ */
