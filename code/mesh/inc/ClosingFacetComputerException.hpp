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
        ClosingFacetComputerException(const char* s) :
               Exception(s)
        {
        }
};

#endif  /* CLOSINGFACETCOMPUTEREXCEPTION_HPP_ */
