/*
 * OutputTransformerException.hpp
 *
 *  Created on: Jul 1, 2014
 *      Author: cady
 */

#ifndef OUTPUTTRANSFORMEREXCEPTION_HPP_
#define OUTPUTTRANSFORMEREXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class OutputTransformerException: public Exception
{
    public:
        OutputTransformerException(const char* s) :
                Exception(s)
        {
        }
};

#endif /* OUTPUTTRANSFORMEREXCEPTION_HPP_ */
