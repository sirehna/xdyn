/*
 * EnvironmentAndFrames.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#ifndef ENVIRONMENTANDFRAMES_HPP_
#define ENVIRONMENTANDFRAMES_HPP_

#include "tr1_macros.hpp"
#include TR1INC(memory)

class Kinematics;
typedef TR1(shared_ptr)<Kinematics> KinematicsPtr;
class WaveModelInterface;
typedef TR1(shared_ptr)<WaveModelInterface> WaveModelPtr;

struct EnvironmentAndFrames
{
    EnvironmentAndFrames();
    WaveModelPtr w;
    KinematicsPtr k;
    double rho;
    double g;
};

#endif /* ENVIRONMENTANDFRAMES_HPP_ */
