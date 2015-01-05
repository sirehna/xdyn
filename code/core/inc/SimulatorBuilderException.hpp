/*
 * SimulatorBuilderException.hpp
 *
 *  Created on: Jun 18, 2014
 *      Author: cady
 */

#ifndef SIMULATORBUILDEREXCEPTION_HPP_
#define SIMULATORBUILDEREXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class SimulatorBuilderException: public ::ssc::exception_handling::Exception
{
    public:
        SimulatorBuilderException(const char* s) :
            ::ssc::exception_handling::Exception(s)
        {
        }
};

#endif /* SIMULATORBUILDEREXCEPTION_HPP_ */
