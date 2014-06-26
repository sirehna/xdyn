/*
 * SimulatorBuilderException.hpp
 *
 *  Created on: Jun 18, 2014
 *      Author: cady
 */

#ifndef SIMULATORBUILDEREXCEPTION_HPP_
#define SIMULATORBUILDEREXCEPTION_HPP_

#include "Exception.hpp"

class SimulatorBuilderException: public Exception
{
    public:
        SimulatorBuilderException(const char* s) :
                Exception(s)
        {
        }
};

#endif /* SIMULATORBUILDEREXCEPTION_HPP_ */
