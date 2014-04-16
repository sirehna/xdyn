/*
 * SimulatorYamlParserException.hpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#ifndef SIMULATORYAMLPARSEREXCEPTION_HPP_
#define SIMULATORYAMLPARSEREXCEPTION_HPP_

#include "Exception.hpp"

class SimulatorYamlParserException : public Exception
{
    public:
        SimulatorYamlParserException(const char* s) :
                Exception(s)
        {
        }
};



#endif /* SIMULATORYAMLPARSEREXCEPTION_HPP_ */
