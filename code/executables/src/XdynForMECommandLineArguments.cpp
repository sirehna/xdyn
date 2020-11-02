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
                         verbose(false),
                         show_help(false),
                         show_websocket_debug_information(false),
                         grpc(false)
{
}

bool XdynForMECommandLineArguments::empty() const
{
    return yaml_filenames.empty() and (port == 0);
}


