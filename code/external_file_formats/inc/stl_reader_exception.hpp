#ifndef STL_READER_EXCEPTION_HPP_
#define STL_READER_EXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class StlReaderException : public ::ssc::exception_handling::Exception
{
    public:
        StlReaderException(const std::string& message, const std::string& file, const std::string& function, const unsigned int line) :
            ::ssc::exception_handling::Exception(message, file, function, line)
        {
        }
};

#endif  /* STLREADEREXCEPTION_HPP_ */
