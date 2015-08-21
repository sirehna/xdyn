#ifndef DAMPINGMATRIXINTERPOLATOREXCEPTION_HPP_
#define DAMPINGMATRIXINTERPOLATOREXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class DampingMatrixInterpolatorException: public ssc::exception_handling::Exception
{
    public:
        DampingMatrixInterpolatorException(const std::string& message, const std::string& file, const std::string& function, const unsigned int line) :
            ::ssc::exception_handling::Exception(message, file, function, line)
        {
        }
};

#endif // DAMPINGMATRIXINTERPOLATOREXCEPTION_HPP_
