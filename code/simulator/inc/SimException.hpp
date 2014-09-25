#ifndef SIMEXCEPTION_HPP_
#define SIMEXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class SimException: public Exception
{
    public:
        SimException(const char* s) :
                Exception(s)
        {
        }
};

#endif /* SIMEXCEPTION_HPP_ */
