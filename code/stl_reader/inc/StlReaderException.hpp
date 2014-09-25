#ifndef STLREADEREXCEPTION_HPP_
#define STLREADEREXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class StlReaderException : public Exception
{
    public:
        StlReaderException(const char* s) :
                Exception(s)
        {
        }
};

#endif  /* STLREADEREXCEPTION_HPP_ */
