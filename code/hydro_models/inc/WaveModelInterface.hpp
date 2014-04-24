/*
 * WaveModelInterface.hpp
 *
 *  Created on: 22 avr. 2014
 *      Author: cady
 */

#ifndef WAVEMODELINTERFACE_HPP_
#define WAVEMODELINTERFACE_HPP_

class Point;

/** \author cec
 *  \date 24 avr. 2014, 10:28:25
 *  \brief Interface to wave models
 *  \details It is this base class' responsibility to change the reference frame of the input point.
 *  \ingroup hydro_models
 *  \section ex1 Example
 *  \snippet hydro_models/unit_tests/src/WaveModelInterfaceTest.cpp WaveModelInterfaceTest example
 *  \section ex2 Expected output
 *  \snippet hydro_models/unit_tests/src/WaveModelInterfaceTest.cpp WaveModelInterfaceTest expected output
 */
class WaveModelInterface
{
    public:
        virtual ~WaveModelInterface() {}
        virtual double get_relative_wave_height(const Point& P) const = 0;
};

#endif /* WAVEMODELINTERFACE_HPP_ */
