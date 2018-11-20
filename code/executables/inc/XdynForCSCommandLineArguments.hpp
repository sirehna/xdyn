/*
 * XdynForCSCommandLineArguments.hpp
 *
 *  Created on: Nov 14, 2018
 *      Author: cady
 */

#ifndef EXECUTABLES_INC_XDYNFORCSCOMMANDLINEARGUMENTS_HPP_
#define EXECUTABLES_INC_XDYNFORCSCOMMANDLINEARGUMENTS_HPP_

#include "XdynCommandLineArguments.hpp"

struct XdynForCSCommandLineArguments
{
    XdynForCSCommandLineArguments();
    bool empty() const;
    std::vector<std::string> yaml_filenames;
    std::string solver;
    double initial_timestep;
    bool catch_exceptions;
    short unsigned int port;
    bool show_help;
};


#endif /* EXECUTABLES_INC_XDYNFORCSCOMMANDLINEARGUMENTS_HPP_ */
