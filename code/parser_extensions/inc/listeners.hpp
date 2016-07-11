/*
 * listeners.hpp
 *
 *  Created on: Oct 21, 2014
 *      Author: cady
 */

#ifndef LISTENERS_HPP_
#define LISTENERS_HPP_

#include <ssc/data_source.hpp>

#include "YamlCommands.hpp"

/**  \brief Reads data from YAML & builds an interpolation table per command.
  *  \returns DataSource used to retrieve the commands of the controlled forces models at each instant
  *  \snippet parser_extensions/unit_tests/src/listenersTest.cpp listenersTest listen_to_file_example
  */
ssc::data_source::DataSource make_command_listener(const std::vector<YamlCommands>& commands);

#endif /* LISTENERS_HPP_ */
