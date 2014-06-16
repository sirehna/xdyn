/*
 * DataSourceSerializerException.hpp
 *
 *  Created on: 28 feb 2014
 *      Author: cec
 */

#ifndef DATASOURCESERIALIZEREXCEPTION_HPP_
#define DATASOURCESERIALIZEREXCEPTION_HPP_


#include "Exception.hpp"

class DataSourceSerializerException : public Exception
{
    public:
        DataSourceSerializerException(const char* s) :
                Exception(s)
        {
        }
};


#endif /* DATASOURCESERIALIZEREXCEPTION_HPP_ */
