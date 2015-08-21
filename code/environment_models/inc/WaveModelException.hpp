/*
 * WaveSpectralDensityException.hpp
 *
 *  Created on: Aug 1, 2014
 *      Author: cady
 */

#ifndef WAVEMODELEXCEPTION_HPP_
#define WAVEMODELEXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

class WaveModelException: public ::ssc::exception_handling::Exception
{
    public:
        WaveModelException(const std::string& message, const std::string& file, const std::string& function, const unsigned int line) :
            ::ssc::exception_handling::Exception(message, file, function, line)
        {
        }
};

#endif /* WAVEMODELEXCEPTION_HPP_ */
