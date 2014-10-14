/*
 * MeshIntersectorException.hpp
 *
 *  Created on: Oct 14, 2014
 *      Author: cady
 */

#ifndef MESHINTERSECTOREXCEPTION_HPP_
#define MESHINTERSECTOREXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class MeshIntersectorException: public Exception
{
    public:
        MeshIntersectorException(const char* s) :
                Exception(s)
        {
        }
};

#endif  /* MESHINTERSECTOREXCEPTION_HPP_ */
