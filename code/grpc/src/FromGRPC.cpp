/*
 * FromGRPC.cpp
 *
 *  Created on: Jun 26, 2019
 *      Author: cady
 */

#include "FromGRPC.hpp"

WaveRequest FromGRPC::to_wave_request(const RequiredWaveInformationResponse& response) const
{
    WaveRequest ret;
    ret.angular_frequencies_for_rao = response.angular_frequencies_for_rao();
    ret.directions_for_rao = response.directions_for_rao();
    ret.dynamic_pressures.t = response.dynamic_pressures().t();
    ret.dynamic_pressures.x.reserve(response.dynamic_pressures().x_size());
    std::copy(response.dynamic_pressures().x().begin(), response.dynamic_pressures().x().end(), std::back_inserter(ret.dynamic_pressures.x));
    ret.dynamic_pressures.y.reserve(response.dynamic_pressures().y_size());
    std::copy(response.dynamic_pressures().y().begin(), response.dynamic_pressures().y().end(), std::back_inserter(ret.dynamic_pressures.y));
    ret.elevations.t = response.elevations().t();
    ret.elevations.x.reserve(response.elevations().x_size());
    std::copy(response.elevations().x().begin(), response.elevations().x().end(), std::back_inserter(ret.elevations.x));
    ret.elevations.y.reserve(response.elevations().y_size());
    std::copy(response.elevations().y().begin(), response.elevations().y().end(), std::back_inserter(ret.elevations.y));
    ret.need_spectrum = response.need_spectrum();
    ret.orbital_velocities.t = response.orbital_velocities().t();
    ret.orbital_velocities.x.reserve(response.orbital_velocities().x_size());
    std::copy(response.orbital_velocities().x().begin(), response.orbital_velocities().x().end(), std::back_inserter(ret.orbital_velocities.x));
    ret.orbital_velocities.y.reserve(response.orbital_velocities().y_size());
    std::copy(response.orbital_velocities().y().begin(), response.orbital_velocities().y().end(), std::back_inserter(ret.orbital_velocities.y));
    ret.orbital_velocities.z.reserve(response.orbital_velocities().z_size());
    std::copy(response.orbital_velocities().z().begin(), response.orbital_velocities().z().end(), std::back_inserter(ret.orbital_velocities.z));
    ret.spectrum.t = response.spectrum().t();
    ret.spectrum.x = response.spectrum().x();
    ret.spectrum.y = response.spectrum().y();
    return ret;
}

ssc::kinematics::Vector6d FromGRPC::to_force(const ForceResponse& response) const
{
    ssc::kinematics::Vector6d ret;
    ret(0) = response.fx();
    ret(1) = response.fy();
    ret(2) = response.fz();
    ret(3) = response.mx();
    ret(4) = response.my();
    ret(5) = response.mz();
    return ret;
}
