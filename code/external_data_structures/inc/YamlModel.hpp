/*
 * YamlModel.hpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#ifndef YAMLMODEL_HPP_
#define YAMLMODEL_HPP_

#include <string>

struct YamlModel
{
    YamlModel();
    std::string model;
    std::string yaml;
    size_t      index_of_first_line_in_global_yaml; //!< Because the force parsers will treat the yaml as a new document so we provide an offset to help diagnosis
};

#endif /* YAMLMODEL_HPP_ */
