/*
 * DiffractionForceModelException.hpp
 *
 *  Created on: Dec 18, 2014
 *      Author: cady
 */

#ifndef DIFFRACTIONFORCEMODELEXCEPTION_HPP_
#define DIFFRACTIONFORCEMODELEXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class DiffractionForceModelException: public ssc::exception_handling::Exception
{
    public:
        DiffractionForceModelException(const std::string& message, const std::string& file, const std::string& function, const unsigned int line) :
            ::ssc::exception_handling::Exception(message, file, function, line)
        {
        }
};

#endif  /* DIFFRACTIONFORCEMODELEXCEPTION_HPP_ */
