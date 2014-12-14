/*
 * hdb_test.hpp
 *
 *  Created on: Dec 14, 2014
 *      Author: cady
 */

#ifndef HDB_TEST_HPP_
#define HDB_TEST_HPP_

#include <functional>

std::function<double(double)> get_interpolated_Br();
std::function<double(double)> get_interpolated_K();

#endif /* HDB_TEST_HPP_ */
