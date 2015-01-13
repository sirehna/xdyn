/*
 * ObserverException.hpp
 *
 *  Created on: Jan 13, 2015
 *      Author: cady
 */


#ifndef OBSERVEREXCEPTION_HPP_
#define OBSERVEREXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class ObserverException : public ssc::exception_handling::Exception
{
    public:
        ObserverException(const char* s) :
               Exception(s)
        {
        }
};

#endif  /* OBSERVEREXCEPTION_HPP_ */
