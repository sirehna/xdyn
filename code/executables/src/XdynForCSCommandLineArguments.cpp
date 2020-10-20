/*
 * XdynForCSCommandLineArguments.cpp
 *
 *  Created on: Nov 14, 2018
 *      Author: cady
 */


#include "XdynForCSCommandLineArguments.hpp"

XdynForCSCommandLineArguments::XdynForCSCommandLineArguments() : yaml_filenames(),
solver(), initial_timestep(), catch_exceptions(), port(0), verbose(false), show_help(false), show_websocket_debug_information(false), grpc(false)
{
}

bool XdynForCSCommandLineArguments::empty() const
{
    return yaml_filenames.empty() and (initial_timestep == 0) and (port == 0);
}
