/*
 * InputData.cpp
 *
 *  Created on: Jun 24, 2014
 *      Author: cady
 */

#include "InputData.hpp"

InputData::InputData() : yaml_filenames(std::vector<std::string>()),
                         output_csv(std::string()),
                         solver(std::string()),
                         wave_output(std::string()),
                         initial_timestep(0),
                         tstart(0),
                         tend(0),
                         help(false)
{
}

