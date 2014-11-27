/*
 * forces_parsers.hpp
 *
 *  Created on: Apr 28, 2014
 *      Author: cady
 */

#ifndef FORCES_PARSERS_HPP_
#define FORCES_PARSERS_HPP_

#include <string>

#include <Eigen/Dense>

#include "YamlGravity.hpp"
#include "YamlWageningen.hpp"
#include "YamlResistanceCurve.hpp"


YamlGravity parse_gravity(const std::string& yaml);
double parse_hydrostatic(const std::string& yaml);
Eigen::Matrix<double,6,6> parse_quadratic_damping(const std::string& yaml);
YamlWageningen parse_wageningen(const std::string& yaml);
YamlResistanceCurve parse_resistance_curve(const std::string& yaml);

#endif /* FORCES_PARSERS_HPP_ */
