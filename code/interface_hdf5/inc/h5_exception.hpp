#ifndef H5_EXCEPTION_HPP
#define H5_EXCEPTION_HPP

#include "ssc/exception_handling.hpp"
class H5Exception: public ssc::exception_handling::Exception
{
    public:
        H5Exception(const std::string& message, const std::string& file, const std::string& function, const unsigned int line) :
            ::ssc::exception_handling::Exception(message, file, function, line)
        {
        }
};

#endif
