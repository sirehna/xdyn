#ifndef BLOCKEDDOFEXCEPTION_HPP_
#define BLOCKEDDOFEXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class BlockedDOFException: public ssc::exception_handling::Exception
{
    public:
        BlockedDOFException(const std::string& message, const std::string& file, const std::string& function, const unsigned int line) :
            ::ssc::exception_handling::Exception(message, file, function, line)
        {
        }
};

#endif // BLOCKEDDOFEXCEPTION_HPP_
