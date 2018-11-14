/*
 * XdynForCSCommandLineArguments.cpp
 *
 *  Created on: Nov 14, 2018
 *      Author: cady
 */


#include "XdynForCSCommandLineArguments.hpp"

XdynForCSCommandLineArguments::XdynForCSCommandLineArguments() : port(0)
{
}

bool XdynForCSCommandLineArguments::empty() const
{
    return wave_output.empty() and output_filename.empty()
            and yaml_filenames.empty() and (initial_timestep == 0) and (tstart == 0) and (tend == 0)
            and (port == 0);
}
