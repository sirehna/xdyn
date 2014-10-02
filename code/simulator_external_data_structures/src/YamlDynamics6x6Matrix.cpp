/*
 * YamlInertiaMatrix.cpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#include "YamlDynamics6x6Matrix.hpp"

YamlDynamics6x6Matrix::YamlDynamics6x6Matrix() : frame(""),
                                                 row_1(std::vector<double>()),
                                                 row_2(std::vector<double>()),
                                                 row_3(std::vector<double>()),
                                                 row_4(std::vector<double>()),
                                                 row_5(std::vector<double>()),
                                                 row_6(std::vector<double>())
{}
