#ifndef H5_EXCEPTION_HPP
#define H5_EXCEPTION_HPP

#include "ssc/exception_handling.hpp"
class H5Exception: public ssc::exception_handling::Exception
{
    public:
        H5Exception(const char* s) :
                Exception(s)
        {
        }
};

#endif
