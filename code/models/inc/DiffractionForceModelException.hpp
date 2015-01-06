/*
 * DiffractionForceModelException.hpp
 *
 *  Created on: Dec 18, 2014
 *      Author: cady
 */

#ifndef DIFFRACTIONFORCEMODELEXCEPTION_HPP_
#define DIFFRACTIONFORCEMODELEXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class DiffractionForceModelException: public ssc::exception_handling::Exception
{
    public:
        DiffractionForceModelException(const char* s) :
                Exception(s)
        {
        }
};

#endif  /* DIFFRACTIONFORCEMODELEXCEPTION_HPP_ */
