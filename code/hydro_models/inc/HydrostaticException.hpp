/*
 * HydrostaticException.hpp
 *
 *  Created on: Apr 30, 2014
 *      Author: cady
 */

#ifndef HYDROSTATICEXCEPTION_HPP_
#define HYDROSTATICEXCEPTION_HPP_

#include "Exception.hpp"

class HydrostaticException: public Exception
{
    public:
        HydrostaticException(const char* s) :
                Exception(s)
        {
        }
};


#endif /* HYDROSTATICEXCEPTION_HPP_ */
