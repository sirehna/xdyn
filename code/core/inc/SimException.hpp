#ifndef SIMEXCEPTION_HPP_
#define SIMEXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class SimException: public ::ssc::exception_handling::Exception
{
    public:
        SimException(const char* s) :
            ::ssc::exception_handling::Exception(s)
        {
        }
};

#endif /* SIMEXCEPTION_HPP_ */
