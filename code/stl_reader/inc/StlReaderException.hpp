#ifndef STLREADEREXCEPTION_HPP_
#define STLREADEREXCEPTION_HPP_

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
