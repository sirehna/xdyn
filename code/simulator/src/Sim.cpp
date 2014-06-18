/*
 * Sim.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include "Sim.hpp"

Sim::Sim(const std::vector<Body>& bodies_,
         const std::vector<ListOfForces>& forces_,
         const KinematicsPtr& k_) : bodies(bodies_), forces(forces_), k(k_)
{
}
