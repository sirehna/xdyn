/*
 * WageningenControlledForceModelException.hpp
 *
 *  Created on: Oct 23, 2014
 *      Author: cady
 */

#ifndef WAGENINGENCONTROLLEDFORCEMODELEXCEPTION_HPP_
#define WAGENINGENCONTROLLEDFORCEMODELEXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class WageningenControlledForceModelException: public ssc::exception_handling::Exception
{
    public:
        WageningenControlledForceModelException(const char* s) :
                Exception(s)
        {
        }
};

#endif /* WAGENINGENCONTROLLEDFORCEMODELEXCEPTION_HPP_ */
