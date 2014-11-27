#ifndef DAMPINGMATRIXINTERPOLATOREXCEPTION_HPP_
#define DAMPINGMATRIXINTERPOLATOREXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class DampingMatrixInterpolatorException: public ssc::exception_handling::Exception
{
    public:
        DampingMatrixInterpolatorException(const char* s) :
                Exception(s)
        {
        }
};

#endif // DAMPINGMATRIXINTERPOLATOREXCEPTION_HPP_
