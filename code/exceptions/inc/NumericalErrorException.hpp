#ifndef EXCEPTIONS_INC_NUMERICALERROREXCEPTION_HPP_
#define EXCEPTIONS_INC_NUMERICALERROREXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class NumericalErrorException: public ssc::exception_handling::Exception
{
    public:
        NumericalErrorException(const std::string& message, const std::string& file, const std::string& function, const unsigned int line) :
               Exception(message, file, function, line)
        {
        }
};

#endif // EXCEPTIONS_INC_NUMERICALERROREXCEPTION_HPP_
