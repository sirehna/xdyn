/*
 * ClosingFacetComputerException.hpp
 *
 *  Created on: Mar 2, 2015
 *      Author: cady
 */


#ifndef CLOSINGFACETCOMPUTEREXCEPTION_HPP_
#define CLOSINGFACETCOMPUTEREXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class ClosingFacetComputerException: public ssc::exception_handling::Exception
{
    public:
        ClosingFacetComputerException(const std::string& message, const std::string& file, const std::string& function, const unsigned int line) :
            ::ssc::exception_handling::Exception(message, file, function, line)
        {
        }
};

#endif  /* CLOSINGFACETCOMPUTEREXCEPTION_HPP_ */
