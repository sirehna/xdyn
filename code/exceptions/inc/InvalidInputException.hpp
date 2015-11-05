/*
 * InvalidInputException.hpp
 *
 *  Created on: Nov 4, 2015
 *      Author: cady
 */


#ifndef CORE_INC_INVALIDINPUTEXCEPTION_HPP_
#define CORE_INC_INVALIDINPUTEXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class InvalidInputException: public ssc::exception_handling::Exception
{
    public:
        InvalidInputException(const std::string& message, const std::string& file, const std::string& function, const unsigned int line) :
               Exception(message, file, function, line)
        {
        }
};

#endif  /* CORE_INC_INVALIDINPUTEXCEPTION_HPP_ */
