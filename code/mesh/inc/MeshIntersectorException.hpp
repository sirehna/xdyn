/*
 * MeshIntersectorException.hpp
 *
 *  Created on: Oct 14, 2014
 *      Author: cady
 */

#ifndef MESHINTERSECTOREXCEPTION_HPP_
#define MESHINTERSECTOREXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class MeshIntersectorException: public ::ssc::exception_handling::Exception
{
    public:
        MeshIntersectorException(const std::string& message, const std::string& file, const std::string& function, const unsigned int line) :
            ::ssc::exception_handling::Exception(message, file, function, line)
        {
        }
};

#endif  /* MESHINTERSECTOREXCEPTION_HPP_ */
