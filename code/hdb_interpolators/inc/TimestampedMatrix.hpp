/*
 * TimestampedMatrix.hpp
 *
 *  Created on: Nov 13, 2014
 *      Author: cady
 */

#ifndef TIMESTAMPEDMATRIX_HPP_
#define TIMESTAMPEDMATRIX_HPP_

#include <array>
#include <utility> // std::pair
#include <vector>

typedef std::array<double,6> line;
typedef std::array<line,6> Matrix;
typedef std::pair<double,Matrix> TimestampedMatrix;
typedef std::vector<TimestampedMatrix> TimestampedMatrices;

struct RAOData
{
    RAOData() : periods(), psi(), values() {}
    std::vector<double> periods;
    std::vector<double> psi;
    std::array<std::vector<std::vector<double> >, 6 > values;
};
#endif /* TIMESTAMPEDMATRIX_HPP_ */
