/*
 * YamlRotation.hpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#ifndef YAMLROTATION_HPP_
#define YAMLROTATION_HPP_

#include <string>
#include <vector>

struct YamlRotation
{
    YamlRotation();
    YamlRotation(const std::string& order_by_,
                 const std::vector<std::string>& convention_);
    std::string order_by;
    std::vector<std::string> convention;
};

#endif /* YAMLROTATION_HPP_ */
