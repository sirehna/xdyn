/*
 * ControllableForceModelException.hpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#ifndef CONTROLLABLEFORCEMODELEXCEPTION_HPP_
#define CONTROLLABLEFORCEMODELEXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class ControllableForceModelException: public ssc::exception_handling::Exception
{
    public:
        ControllableForceModelException(const char* s) :
                Exception(s)
        {
        }
};

#endif  /* CONTROLLABLEFORCEMODELEXCEPTION_HPP_ */
