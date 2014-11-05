/*
 * YamlResistanceCurve.hpp
 *
 *  Created on: Oct 24, 2014
 *      Author: cady
 */

#ifndef YAMLRESISTANCECURVE_HPP_
#define YAMLRESISTANCECURVE_HPP_

#include <vector>

struct YamlResistanceCurve
{
    YamlResistanceCurve();
    std::vector<double> Va;
    std::vector<double> R;
};

#endif /* YAMLRESISTANCECURVE_HPP_ */
