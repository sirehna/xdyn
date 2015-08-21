#ifndef SIMEXCEPTION_HPP_
#define SIMEXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class SimException: public ::ssc::exception_handling::Exception
{
    public:
        SimException(const std::string& message, const std::string& file, const std::string& function, const unsigned int line) :
            ::ssc::exception_handling::Exception(message, file, function, line)
        {
        }
};

#endif /* SIMEXCEPTION_HPP_ */
