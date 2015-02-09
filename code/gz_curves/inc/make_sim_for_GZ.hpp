/*
 * make_sim_for_GZ.hpp
 *
 *  Created on: Feb 9, 2015
 *      Author: cady
 */

#ifndef MAKE_SIM_FOR_GZ_HPP_
#define MAKE_SIM_FOR_GZ_HPP_

#include "Sim.hpp"

namespace GZ
{
    Sim make_sim(const std::string& yaml, const std::string& stl);
}
#endif  /* MAKE_SIM_FOR_GZ_HPP_ */
