/*
 * FrameException.hpp
 *
 *  Created on: 4 avr. 2014
 *      Author: maroff
 */

#ifndef FRAMEEXCEPTION_HPP_
#define FRAMEEXCEPTION_HPP_

#include "Exception.hpp"

class FrameException: public Exception
{
    public:
        FrameException(const char* s) :
                Exception(s)
        {
        }
};


#endif /* FRAMEEXCEPTION_HPP_ */
