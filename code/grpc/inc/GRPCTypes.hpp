/*
 * GRPCTypes.hpp
 *
 *  Created on: Jun 26, 2019
 *      Author: cady
 */

#ifndef GRPC_INC_GRPCTYPES_HPP_
#define GRPC_INC_GRPCTYPES_HPP_

#include <vector>

struct XYTs
{
    std::vector<double> x;
    std::vector<double> y;
    double t;
};

struct XYZTs
{
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> z;
    double t;
};

struct XYT
{
    double x;
    double y;
    double t;
};

struct WaveRequest
{
    XYTs elevations;
    XYZTs dynamic_pressures;
    XYZTs orbital_velocities;
    XYT spectrum;
    bool angular_frequencies_for_rao;
    bool directions_for_rao;
    bool need_spectrum;
};


#endif /* GRPC_INC_GRPCTYPES_HPP_ */
