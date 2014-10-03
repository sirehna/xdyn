/*
 * forces_parsers.hpp
 *
 *  Created on: Apr 28, 2014
 *      Author: cady
 */

#ifndef FORCES_PARSERS_HPP_
#define FORCES_PARSERS_HPP_

#include <Eigen/Dense>
#include "YamlForces.hpp"
#include <string>

GravityParameters parse_gravity(const std::string& yaml);
double parse_hydrostatic(const std::string& yaml);
Eigen::Matrix<double,6,6> parse_quadratic_damping(const std::string& yaml);

#endif /* FORCES_PARSERS_HPP_ */
