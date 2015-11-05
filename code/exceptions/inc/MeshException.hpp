#ifndef MESHEXCEPTION_HPP_
#define MESHEXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class MeshException : public ::ssc::exception_handling::Exception
{
    public:
        MeshException(const std::string& message, const std::string& file, const std::string& function, const unsigned int line) :
            ::ssc::exception_handling::Exception(message, file, function, line)
        {
        }
};

#endif
