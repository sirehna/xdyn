#ifndef MESHEXCEPTION_HPP_
#define MESHEXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class MeshException : public Exception
{
    public:
        MeshException(const char* s) :
                Exception(s)
        {
        }
};

#endif
