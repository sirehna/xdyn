/*
 * HDBBuilderException.hpp
 *
 *  Created on: Nov 13, 2014
 *      Author: cady
 */

#ifndef HDBBUILDEREXCEPTION_HPP_
#define HDBBUILDEREXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class HDBBuilderException: public ssc::exception_handling::Exception
{
    public:
        HDBBuilderException(const char* s) :
                Exception(s)
        {
        }
};

#endif  /* HDBBUILDEREXCEPTION_HPP_ */
