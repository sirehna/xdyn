/*
 * simulator_interface.hpp
 *
 *  Created on: 4 mars 2014
 *      Author: cec
 */

#ifndef SIMULATOR_INTERFACE_HPP_
#define SIMULATOR_INTERFACE_HPP_

#include "DataSource.hpp"

DataSource make_ds(const std::string& yaml);
void integrate(DataSource& ds, const double t0, const double t1);

#endif /* SIMULATOR_INTERFACE_HPP_ */
