/*
 * YamlDynamics6x6Matrix.hpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#ifndef YAMLDYNAMICS6X6MATRIX_HPP_
#define YAMLDYNAMICS6X6MATRIX_HPP_

#include <string>
#include <vector>

struct YamlDynamics6x6Matrix
{
    YamlDynamics6x6Matrix();
    bool read_from_file;
    std::string hdb_filename;
    std::string frame;
    std::vector<double> row_1;
    std::vector<double> row_2;
    std::vector<double> row_3;
    std::vector<double> row_4;
    std::vector<double> row_5;
    std::vector<double> row_6;
};

#endif /* YAMLDYNAMICS6X6MATRIX_HPP_ */
