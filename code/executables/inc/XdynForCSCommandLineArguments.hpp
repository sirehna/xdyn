/*
 * XdynForCSCommandLineArguments.hpp
 *
 *  Created on: Nov 14, 2018
 *      Author: cady
 */

#ifndef EXECUTABLES_INC_XDYNFORCSCOMMANDLINEARGUMENTS_HPP_
#define EXECUTABLES_INC_XDYNFORCSCOMMANDLINEARGUMENTS_HPP_

#include "XdynCommandLineArguments.hpp"

struct XdynForCSCommandLineArguments : XdynCommandLineArguments
{
    XdynForCSCommandLineArguments();
    bool empty() const;
    short unsigned int port;

};


#endif /* EXECUTABLES_INC_XDYNFORCSCOMMANDLINEARGUMENTS_HPP_ */
