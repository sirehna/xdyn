/*
 * InputData.cpp
 *
 *  Created on: Jun 24, 2014
 *      Author: cady
 */

#include "InputData.hpp"

InputData::InputData() : yaml_filenames(),
                         solver(),
                         output_filename(),
                         wave_output(),
                         command_file(),
                         initial_timestep(0),
                         tstart(0),
                         tend(0),
                         help(false)
{
}

bool InputData::empty() const
{
    return wave_output.empty() and command_file.empty() and output_filename.empty()
            and yaml_filenames.empty() and (initial_timestep == 0) and (tstart == 0) and (tend == 0)
            and (not(help));
}
