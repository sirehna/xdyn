/*
 * ControllableForceModelException.hpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#ifndef CONTROLLABLEFORCEMODELEXCEPTION_HPP_
#define CONTROLLABLEFORCEMODELEXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class ControllableForceModelException: public ssc::exception_handling::Exception
{
    public:
        ControllableForceModelException(const std::string& message, const std::string& file, const std::string& function, const unsigned int line) :
            ::ssc::exception_handling::Exception(message, file, function, line)
        {
        }
};

#endif  /* CONTROLLABLEFORCEMODELEXCEPTION_HPP_ */
