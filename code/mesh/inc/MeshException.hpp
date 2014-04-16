#ifndef MESHEXCEPTION_HPP_
#define MESHEXCEPTION_HPP_

#include "Exception.hpp"

class MeshException : public Exception
{
    public:
        MeshException(const char* s) :
                Exception(s)
        {
        }
};

#endif
