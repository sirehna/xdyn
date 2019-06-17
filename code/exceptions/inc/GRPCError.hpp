/*
 * GRPCException.hpp
 *
 *  Created on: May 28, 2019
 *      Author: cady
 */

#ifndef EXCEPTIONS_INC_GRPCEXCEPTION_HPP_
#define EXCEPTIONS_INC_GRPCEXCEPTION_HPP_


#include <ssc/exception_handling.hpp>

class GRPCError: public ssc::exception_handling::Exception
{
    public:
        GRPCError(const std::string& message, const std::string& file, const std::string& function, const unsigned int line) :
               Exception(message, file, function, line)
        {
        }
};


#endif /* EXCEPTIONS_INC_GRPCEXCEPTION_HPP_ */
