#ifndef RADIATIONDAMPINGFORCEMODELEXCEPTION_HPP_
#define RADIATIONDAMPINGFORCEMODELEXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class RadiationDampingForceModelException: public ssc::exception_handling::Exception
{
    public:
        RadiationDampingForceModelException(const char* s) :
                Exception(s)
        {
        }
};

#endif // RADIATIONDAMPINGFORCEMODELEXCEPTION_HPP_
