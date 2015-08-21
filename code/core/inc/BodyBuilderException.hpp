/*
 * BodyBuilderException.hpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#ifndef BODYBUILDEREXCEPTION_HPP_
#define BODYBUILDEREXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class BodyBuilderException: public ::ssc::exception_handling::Exception
{
    public:
        BodyBuilderException(const std::string& message, const std::string& file, const std::string& function, const unsigned int line) :
            ::ssc::exception_handling::Exception(message, file, function, line)
        {
        }
};

#endif /* BODYBUILDEREXCEPTION_HPP_ */
