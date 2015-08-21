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
        WageningenControlledForceModelException(const std::string& message, const std::string& file, const std::string& function, const unsigned int line) :
            ::ssc::exception_handling::Exception(message, file, function, line)
        {
        }
};

#endif /* WAGENINGENCONTROLLEDFORCEMODELEXCEPTION_HPP_ */
