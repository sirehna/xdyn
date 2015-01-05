/*
 * YamlWageningen.hpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#ifndef YAMLWAGENINGEN_HPP_
#define YAMLWAGENINGEN_HPP_

#include "YamlPosition.hpp"

struct YamlWageningen
{
    YamlWageningen();
    std::string name;
    YamlPosition position_of_propeller_frame;
    double wake_coefficient;
    double relative_rotative_efficiency;
    double thrust_deduction_factor;
    bool rotating_clockwise;
    size_t number_of_blades;
    double blade_area_ratio;
    double diameter;
};

#endif /* YAMLWAGENINGEN_HPP_ */
