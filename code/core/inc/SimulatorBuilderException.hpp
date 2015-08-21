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
        SimulatorBuilderException(const std::string& message, const std::string& file, const std::string& function, const unsigned int line) :
            ::ssc::exception_handling::Exception(message, file, function, line)
        {
        }
};

#endif /* SIMULATORBUILDEREXCEPTION_HPP_ */
