/*
 * hydrostatic.cpp
 *
 *  Created on: Apr 30, 2014
 *      Author: cady
 */


#include "hydrostatic.hpp"
#include "HydrostaticException.hpp"

#include <algorithm> // std::count_if

using namespace hydrostatic;


bool hydrostatic::totally_immerged(const size_t idx[3], const std::vector<double>& delta_z)
{
    return (delta_z.at(idx[0]) > 0) and (delta_z.at(idx[1]) > 0) and (delta_z.at(idx[2]) > 0);
}

double hydrostatic::average_immersion(const size_t idx[3], const std::vector<double>& delta_z)
{
    return (delta_z.at(idx[0]) + delta_z.at(idx[1]) + delta_z.at(idx[2]))/3.;
}

bool positive (const double x);
bool positive (const double x) { return x>0; }

void make_sure_some_points_are_immerged_and_some_are_not(const size_t idx[3], const std::vector<double>& v);
void make_sure_some_points_are_immerged_and_some_are_not(const size_t idx[3], const std::vector<double>& v)
{
    const size_t n = 3;
    std::vector<double> z(n,0);
    for (size_t i = 0 ; i < n ; ++i)
    {
        z.push_back(v.at(idx[i]));
    }

    const size_t number_of_immerged_nodes = std::count_if(z.begin(), z.end(), positive);

    if (number_of_immerged_nodes == 0)
    {
        THROW(__PRETTY_FUNCTION__, HydrostaticException, "None of the points are immerged.");
    }
    if (number_of_immerged_nodes == n)
    {
        THROW(__PRETTY_FUNCTION__, HydrostaticException, "All the points are immerged.");
    }
}

Eigen::Matrix<double,3,Eigen::Dynamic> hydrostatic::immerged_polygon(const Eigen::Matrix<double,3,Eigen::Dynamic>& M,
                                                                     const size_t idx[3],
                                                                     const std::vector<double>& v)
{
    make_sure_some_points_are_immerged_and_some_are_not(idx, v);
    return M;
}
