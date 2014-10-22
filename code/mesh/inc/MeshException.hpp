#ifndef MESHEXCEPTION_HPP_
#define MESHEXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class MeshException : public ::ssc::exception_handling::Exception
{
    public:
        MeshException(const char* s) :
            ::ssc::exception_handling::Exception(s)
        {
        }
};

#endif
