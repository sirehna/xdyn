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

#include "YamlDiffraction.hpp"
#include "YamlGravity.hpp"
#include "YamlRadiationDamping.hpp"
#include "YamlResistanceCurve.hpp"
#include "YamlWageningen.hpp"

YamlWageningen parse_wageningen(const std::string& yaml);
TypeOfQuadrature parse_type_of_quadrature(const std::string& s);
YamlDiffraction parse_diffraction(const std::string& yaml);

#endif /* FORCES_PARSERS_HPP_ */
