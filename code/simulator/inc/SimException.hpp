#ifndef SIMEXCEPTION_HPP_
#define SIMEXCEPTION_HPP_

#include "Exception.hpp"

class SimException: public Exception
{
    public:
        SimException(const char* s) :
                Exception(s)
        {
        }
};

#endif /* SIMEXCEPTION_HPP_ */
