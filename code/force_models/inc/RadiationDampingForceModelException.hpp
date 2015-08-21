#ifndef RADIATIONDAMPINGFORCEMODELEXCEPTION_HPP_
#define RADIATIONDAMPINGFORCEMODELEXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class RadiationDampingForceModelException: public ssc::exception_handling::Exception
{
    public:
        RadiationDampingForceModelException(const std::string& message, const std::string& file, const std::string& function, const unsigned int line) :
            ::ssc::exception_handling::Exception(message, file, function, line)
        {
        }
};

#endif // RADIATIONDAMPINGFORCEMODELEXCEPTION_HPP_
