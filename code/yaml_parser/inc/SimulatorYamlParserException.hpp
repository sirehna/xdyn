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
        SimulatorYamlParserException(const std::string& message, const std::string& file, const std::string& function, const unsigned int line) :
            ::ssc::exception_handling::Exception(message, file, function, line)
        {
        }
};

#endif /* SIMULATORYAMLPARSEREXCEPTION_HPP_ */
