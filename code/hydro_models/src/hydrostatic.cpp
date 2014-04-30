/*
 * hydrostatic.cpp
 *
 *  Created on: Apr 30, 2014
 *      Author: cady
 */


#include "hydrostatic.hpp"

using namespace hydrostatic;


bool hydrostatic::totally_immerged(const size_t idx[3], const std::vector<double>& delta_z)
{
    return (delta_z.at(idx[0]) > 0) and (delta_z.at(idx[1]) > 0) and (delta_z.at(idx[2]) > 0);
}

double hydrostatic::average_immersion(const size_t idx[3], const std::vector<double>& delta_z)
{
    return (delta_z.at(idx[0]) + delta_z.at(idx[1]) + delta_z.at(idx[2]))/3.;
}
