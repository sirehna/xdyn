/*
 * SimulatorYamlParserException.hpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#ifndef SIMULATORYAMLPARSEREXCEPTION_HPP_
#define SIMULATORYAMLPARSEREXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class SimulatorYamlParserException : public ::ssc::exception_handling::Exception
{
    public:
        SimulatorYamlParserException(const char* s) :
            ::ssc::exception_handling::Exception(s)
        {
        }
};

#endif /* SIMULATORYAMLPARSEREXCEPTION_HPP_ */
