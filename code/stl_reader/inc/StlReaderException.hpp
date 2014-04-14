#ifndef STLREADEREXCEPTION_HPP_
#define STLREADEREXCEPTION_HPP_

#include "Exception.hpp"

class StlReaderException : public Exception
{
    public:
        StlReaderException(const char* s) :
                Exception(s)
        {
        }
};

#endif  /* STLREADEREXCEPTION_HPP_ */
