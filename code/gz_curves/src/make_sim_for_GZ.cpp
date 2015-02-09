/*
 * make_sim_for_GZ.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: cady
 */

#include "make_sim_for_GZ.hpp"
#include "simulator_api.hpp"

Sim GZ::make_sim(const std::string& yaml, const std::string& stl)
{
    ssc::data_source::DataSource ds;
    return get_system(yaml, stl, 0, ds);
}
