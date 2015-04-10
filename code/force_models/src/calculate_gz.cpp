/*
 * GZComputer.cpp
 *
 *  Created on: Mar 26, 2015
 *      Author: cady
 */

#include "calculate_gz.hpp"
#include "EnvironmentAndFrames.hpp"

double calculate_gz(const ssc::kinematics::Transform& body2ned,
                    const ssc::kinematics::Wrench& force_and_torque_projected_in_NED_frame)
{
    const Eigen::Vector3d Xbody_projected_in_NED_frame = body2ned.get_rot()*Eigen::Vector3d(1,0,0);
    const double cos_psi_cos_theta = Xbody_projected_in_NED_frame(0);
    const double sin_psi_cos_theta = Xbody_projected_in_NED_frame(1);
    const double fz = force_and_torque_projected_in_NED_frame.force(2);
    const double mx = force_and_torque_projected_in_NED_frame.torque(0);
    const double my = force_and_torque_projected_in_NED_frame.torque(1);
    return (cos_psi_cos_theta*mx + sin_psi_cos_theta*my)/fz/Xbody_projected_in_NED_frame.norm();
}

double calculate_gz(const ForceModel& F, const EnvironmentAndFrames& env)
{
    return calculate_gz(env.k->get("NED", F.get_body_name()), F.get_force_in_ned_frame());
}
