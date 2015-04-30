/*
 * WebSocketObserverException.hpp
 *
 *  Created on: Apr 30, 2015
 *      Author: cady
 */


#ifndef WEBSOCKETOBSERVEREXCEPTION_HPP_
#define WEBSOCKETOBSERVEREXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class WebSocketException: public ::ssc::exception_handling::Exception
{
    public:
        WebSocketException(const char* s) :
            ::ssc::exception_handling::Exception(s)
        {
        }
};


#endif  /* WEBSOCKETOBSERVEREXCEPTION_HPP_ */
