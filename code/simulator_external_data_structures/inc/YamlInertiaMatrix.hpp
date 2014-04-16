/*
 * YamlInertiaMatrix.hpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#ifndef YAMLINERTIAMATRIX_HPP_
#define YAMLINERTIAMATRIX_HPP_

#include <string>
#include <vector>

struct YamlInertiaMatrix
{
    YamlInertiaMatrix();
    std::string frame;
    std::vector<double> row_1;
    std::vector<double> row_2;
    std::vector<double> row_3;
    std::vector<double> row_4;
    std::vector<double> row_5;
    std::vector<double> row_6;
};

#endif /* YAMLINERTIAMATRIX_HPP_ */
