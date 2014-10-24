/*
 * InputData.cpp
 *
 *  Created on: Jun 24, 2014
 *      Author: cady
 */

#include "InputData.hpp"

InputData::InputData() : yaml_filenames(),
                         output_csv(),
                         solver(),
                         wave_output(),
                         command_file(),
                         initial_timestep(0),
                         tstart(0),
                         tend(0),
                         help(false)
{
}

