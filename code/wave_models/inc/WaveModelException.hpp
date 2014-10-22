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
        WaveModelException(const char* s) :
            ::ssc::exception_handling::Exception(s)
        {
        }
};

#endif /* WAVEMODELEXCEPTION_HPP_ */
