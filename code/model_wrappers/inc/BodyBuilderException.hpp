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
        BodyBuilderException(const char* s) :
            ::ssc::exception_handling::Exception(s)
        {
        }
};

#endif /* BODYBUILDEREXCEPTION_HPP_ */
