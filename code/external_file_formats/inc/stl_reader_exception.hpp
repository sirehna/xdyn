#ifndef STL_READER_EXCEPTION_HPP_
#define STL_READER_EXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class StlReaderException : public ::ssc::exception_handling::Exception
{
    public:
        StlReaderException(const char* s) :
            ::ssc::exception_handling::Exception(s)
        {
        }
};

#endif  /* STLREADEREXCEPTION_HPP_ */
