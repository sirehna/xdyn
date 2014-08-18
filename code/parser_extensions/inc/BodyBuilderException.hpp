/*
 * BodyBuilderException.hpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#ifndef BODYBUILDEREXCEPTION_HPP_
#define BODYBUILDEREXCEPTION_HPP_

#include "Exception.hpp"

class BodyBuilderException: public Exception
{
    public:
        BodyBuilderException(const char* s) :
                Exception(s)
        {
        }
};

#endif /* BODYBUILDEREXCEPTION_HPP_ */
