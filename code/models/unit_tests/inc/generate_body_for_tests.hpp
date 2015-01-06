/*
 * simulator_test.hpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#ifndef SIMULATOR_TEST_HPP_
#define SIMULATOR_TEST_HPP_

#include <string>

#include "Body.hpp"
#include "GeometricTypes3d.hpp"

Body get_body(const std::string& names);
Body get_body(const std::string& name, const VectorOfVectorOfPoints& points);

#endif /* SIMULATOR_TEST_HPP_ */
