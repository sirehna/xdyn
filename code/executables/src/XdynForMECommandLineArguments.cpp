/*
 * InputData.cpp
 *
 *  Created on: Jun 24, 2014
 *      Author: cady
 */

#include "XdynForMECommandLineArguments.hpp"

XdynForMECommandLineArguments::XdynForMECommandLineArguments() : yaml_filenames(),
                         catch_exceptions(false),
                         port(0),
                         show_help(false)
{
}

bool XdynForMECommandLineArguments::empty() const
{
    return yaml_filenames.empty() and (port == 0);
}


