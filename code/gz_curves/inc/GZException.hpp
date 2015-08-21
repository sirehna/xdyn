/*
 * GZException.hpp
 *
 *  Created on: Feb 20, 2015
 *      Author: cady
 */


#ifndef GZEXCEPTION_HPP_
#define GZEXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class GZException: public ssc::exception_handling::Exception
{
    public:
        GZException(const std::string& message, const std::string& file, const std::string& function, const unsigned int line) :
            ::ssc::exception_handling::Exception(message, file, function, line)
        {
        }
};

#endif  /* GZEXCEPTION_HPP_ */
