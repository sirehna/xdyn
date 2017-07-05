/*
 * env_for_tests.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: cady
 */


#include "env_for_tests.hpp"
#include "SurfaceElevationFromWaves.hpp"

EnvironmentAndFrames get_environment_and_frames(const TR1(shared_ptr)<WaveModel>& wave_model)
{
    EnvironmentAndFrames env;
    env.g = 9.81;
    env.rho = 1024;
    env.rot = YamlRotation("angle", {"z","y'","x''"});
    env.k = ssc::kinematics::KinematicsPtr(new ssc::kinematics::Kinematics());
    env.k->add(ssc::kinematics::Transform(ssc::kinematics::Point("NED"), "mesh(" BODY ")"));
    env.k->add(ssc::kinematics::Transform(ssc::kinematics::Point("NED"), BODY));
    TR1(shared_ptr)<ssc::kinematics::PointMatrix> mesh;
    env.w = SurfaceElevationPtr(new SurfaceElevationFromWaves(wave_model));
    return env;
}



