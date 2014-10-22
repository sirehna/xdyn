/*
 * HydrostaticException.hpp
 *
 *  Created on: Apr 30, 2014
 *      Author: cady
 */

#ifndef HYDROSTATICEXCEPTION_HPP_
#define HYDROSTATICEXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class HydrostaticException: public ::ssc::exception_handling::Exception
{
    public:
        HydrostaticException(const char* s) :
            ::ssc::exception_handling::Exception(s)
        {
        }
};


#endif /* HYDROSTATICEXCEPTION_HPP_ */
