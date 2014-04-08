/*
 * KinematicsException.hpp
 *
 *  Created on: 7 avr. 2014
 *      Author: maroff
 */

#ifndef KINEMATICSEXCEPTION_HPP_
#define KINEMATICSEXCEPTION_HPP_

#include "Exception.hpp"

class KinematicsException: public Exception
{
    public:
        KinematicsException(const char* s) :
                Exception(s)
        {
        }
};


#endif /* KINEMATICSEXCEPTION_HPP_ */
