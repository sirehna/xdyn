/*
 * DataSourceBuilderException.hpp
 *
 *  Created on: May 23, 2014
 *      Author: cady
 */

#ifndef DATASOURCEBUILDEREXCEPTION_HPP_
#define DATASOURCEBUILDEREXCEPTION_HPP_

#include "Exception.hpp"

class DataSourceBuilderException: public Exception
{
    public:
        DataSourceBuilderException(const char* s) :
                Exception(s)
        {
        }
};

#endif /* DATASOURCEBUILDEREXCEPTION_HPP_ */
